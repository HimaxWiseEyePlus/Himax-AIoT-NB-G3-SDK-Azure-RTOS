#include <hx_aiot_nb_g3/inc/azure_iothub.h>
#include <hx_aiot_nb_g3/inc/hii.h>
#include <hx_aiot_nb_g3/inc/pmu.h>
#include <hx_aiot_nb_g3/inc/tflitemicro_algo.h>
#include "spi_master_protocol.h"
#include "hx_drv_tflm.h"
#include "inc/hx_drv_pmu.h"
#include "tx_api.h"
#include "powermode.h"
#include "library/ota/ota.h"
#include "board.h"
#include "BITOPS.h"
#include "external/nb_iot/type1sc/type1sc.h"
//datapath boot up reason flag
volatile uint8_t g_bootup_md_detect = 0;
//#define SPI_SEND

#define IMAGE_HEIGHT 	480
#define IMAGE_WIDTH		640
struct_algoResult algo_result;
uint32_t g_imgsize;
unsigned char *g_img_cur_addr_pos;


#ifdef SPI_SEND
static int open_spi()
{
	int ret ;
#ifndef SPI_MASTER_SEND
	ret = hx_drv_spi_slv_open();
	hx_drv_uart_print("SPI slave ");
#else
	ret = hx_drv_spi_mst_open();
	hx_drv_uart_print("SPI master ");
#endif
    return ret;
}

static int spi_write(uint32_t addr, uint32_t size, SPI_CMD_DATA_TYPE data_type)
{
#ifndef SPI_MASTER_SEND
	return hx_drv_spi_slv_protocol_write_simple_ex(addr, size, data_type);
#else
	return hx_drv_spi_mst_protocol_write_sp(addr, size, data_type);
#endif
}
#endif

hx_drv_sensor_image_config_t g_pimg_config;
static bool is_initialized = false;
int GetImage(int image_width,int image_height, int channels) {
	int ret = 0;
	//xprintf("is_initialized : %d \n",is_initialized);
	  if (!is_initialized) {
	    if (hx_drv_sensor_initial(&g_pimg_config) != HX_DRV_LIB_PASS) {
	    	xprintf("hx_drv_sensor_initial error\n");
	      return ERROR;
	    }
#ifdef SPI_SEND
	    if (hx_drv_spim_init() != HX_DRV_LIB_PASS) {
	      return ERROR;
	    }
	    ret = open_spi();
#endif
	    is_initialized = true;
		xprintf("is_initialized : %d \n",is_initialized);
	  }

	  //capture image by sensor
	  hx_drv_sensor_capture(&g_pimg_config);

	  g_img_cur_addr_pos = (unsigned char *)g_pimg_config.jpeg_address;
	  g_imgsize = g_pimg_config.jpeg_size;
	  xprintf("g_pimg_config.jpeg_address:0x%x size : %d \n",g_pimg_config.jpeg_address,g_pimg_config.jpeg_size);
#ifdef SPI_SEND
	  //send jpeg image data out through SPI
	  ret = spi_write(g_pimg_config.jpeg_address, g_pimg_config.jpeg_size, DATA_TYPE_JPG);
	  //ret = spi_write(g_pimg_config.raw_address, g_pimg_config.raw_size, DATA_TYPE_RAW_IMG);
#endif
	  return OK;
}

int img_cnt = 0;
void tflitemicro_start(){
	img_cnt++;

	GetImage(IMAGE_WIDTH,IMAGE_HEIGHT,1);

#ifdef TFLITE_MICRO_GOOGLE_PERSON
	xprintf("### img_cnt:%d ###\n",img_cnt);
	xprintf("[tflitemicro_algo_run_result]:\n");
	tflitemicro_algo_run(g_pimg_config.raw_address, g_pimg_config.img_width, g_pimg_config.img_height, &algo_result);
	
	xprintf("humanPresence:%d\n",algo_result.humanPresence);
#endif /* TFLITE_MICRO_GOOGLE_PERSON */

	/* azure_active_event
	 * ALGO_EVENT_SEND_RESULT_TO_CLOUD :Send Algorithm Metadata.
	 * ALGO_EVENT_SEND_IMAGE_TO_CLOUD  :Send Image.
	 * ALGO_EVENT_SEND_RESULT_AND_IMAGE:Send Metadata and Image.
	 *
	 * */
	if(algo_result.humanPresence){
		img_cnt= 0;
		azure_active_event = ALGO_EVENT_SEND_RESULT_TO_CLOUD;
#if 0	//Example: Send custom JSON data to Cloud.	
		int size = 0 , ret = 0;
		char *model_hx_aiot_nb_g2 = "{\"human\":1,\"det_box_x\":320,\"det_box_y\":240,\"det_box_width\":50,\"det_box_height\":50,\"image\":0,\"audio\":0,\"accel_x\":18.91,\"accel_y\":18.92,\"accel_z\":18.93,\"gyro_x\":20.91,\"gyro_y\":20.92,\"gyro_z\":20.93,\"mag_x\":20.91,\"mag_y\":20.92,\"mag_z\":20.93}";
		size = strlen(model_hx_aiot_nb_g2);
		xprintf("size:%d",size);
		ret =send_cstm_data_to_cloud(model_hx_aiot_nb_g2, size, SEND_CSTM_JSON_DATA);
		if(ret == -1)
		{	//re-send 1
			ret = send_cstm_data_to_cloud(model_hx_aiot_nb_g2, size, SEND_CSTM_JSON_DATA);

			if(ret == -1){
				//re-send 2
				send_cstm_data_to_cloud(model_hx_aiot_nb_g2, size, SEND_CSTM_JSON_DATA);
			}
		}
#endif	//Example: Send custom JSON data to Cloud.
	}
}

void setup(){
#ifdef TFLITE_MICRO_GOOGLE_PERSON
	xprintf("### TFLITE_MICRO_GOOGLE_PERSON ALGO INITIAL... ###\n");
	tflitemicro_algo_init();
#endif /* TFLITE_MICRO_GOOGLE_PERSON */

#ifdef NB_IOT_BOARD
	/*Azure TX Task. */
	xprintf("#############################################################################\n");
	xprintf("**** Enter TX Thread ****\n");
	xprintf("#############################################################################\n");
	nbiot_task_define();
#endif
}

hx_drv_sensor_image_config_t g_pimg_config;
void hx_aiot_nb_g3()
{
	/* Active Mode. */
	uint32_t addr = 0;
	uint32_t val = 0;
	PM_CFG_T aCfg;
	PM_CFG_PWR_MODE_E mode = PM_MODE_ALL_ON;

	// for external power saving
	addr = 0xB0000074;//  0xB0000074 = 0x0001
	val = _arc_read_uncached_32((void*)addr);
	xprintf("1######val = %d\n", val);

	xprintf("COLD BOOT PMU_WE1_POWERPLAN_EXTERNAL_LDO\n######val = %d\n", val);

	if(!val){
		val = val + 1;//0x0001;
		_arc_write_uncached_32((void *)addr, val);
		xprintf("2######\nval = %d\n", val);
		hx_drv_pmu_set_ctrl(PMU_PWR_PLAN, PMU_WE1_POWERPLAN_EXTERNAL_LDO);
		board_delay_ms(200);
		// disable sldo, 0xb0000410=0x00000001
		hx_lib_pm_sldo_en(0);
		board_delay_ms(200);
		hx_lib_pm_cldo_en(0);
		EnterToPMU(3000);
	}

	if(val){
		xprintf("PMU_WE1_POWERPLAN_EXTERNAL_LDO\n######val = %d\n", val);
		/* External_LDO*/
		hx_drv_pmu_set_ctrl(PMU_PWR_PLAN, PMU_WE1_POWERPLAN_EXTERNAL_LDO);
		board_delay_ms(200);
		// disable sldo, 0xb0000410=0x00000001
		hx_lib_pm_sldo_en(0);
		board_delay_ms(200);
		hx_lib_pm_cldo_en(0);
	}

	setup();
}

