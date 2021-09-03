/*
 * type1sc.c
 *
 *  Created on: 2021/7/1
 *      Author: 903990
 */


#include <type1sc.h>
#include "stdlib.h"
#include "stdio.h"
#include "string.h"
#include "stdarg.h"
#include "embARC_debug.h"
#include "tx_api.h"
#include "hx_drv_iomux.h"

#define UART_READ_CNT 12


/**
* TYPE1SC driver initial
*
* @param uart_id  :uart_no
* @param baudrate :serial baudrate
*
* Return Value:
* DEV_UART_PTR dev_uart_comm
*/
DEV_UART_PTR type1sc_drv_init(USE_SS_UART_E uart_id, uint32_t baudrate){

	DEV_UART_PTR dev_uart_comm = NULL;
	dev_uart_comm = hx_drv_uart_get_dev(uart_id);
	if(dev_uart_comm != NULL){
		dev_uart_comm->uart_open(baudrate);
	}

	return dev_uart_comm;
}

/**
* TYPE1SC driver deinit
*
* @param uart_id:uart_id
*
* Return Value:
* AT_OK, 	deinit success
* AT_ERROR, deinit fail
*/
int8_t type1sc_drv_deinit(USE_SS_UART_E uart_id){

	int ret = hx_drv_uart_deinit(uart_id);

	if(ret !=0){
		xprintf("type1sc_deinit error_no=%d\n",ret);
		return AT_ERROR;
	}

	return AT_OK;
}

/**
* Type1SC LWAN(Low Power Wide Area Network) Setting
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param lpwan_type		:lpwan_type
* - 1: CAT-M
* - 2: NB-IOT
* - 3: GSM
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%RATIMGSEL
*
* Return Value:
*/
int8_t type1sc_drv_lpwan_type_sel(DEV_UART_PTR dev_uart_comm, AT_STRING lpwan_type, char *recv_buf, uint32_t recv_len)
{

	uint8_t uart_read_cnt = 0;

	//AT%RATIMGSEL
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY,"RATIMGSEL",lpwan_type, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[LPWAN Setting]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[LPWAN Setting ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
* Type1SC driver Operators APN(Access Point Name) Setting
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cid			:fixed 1
* @param pdp_type		:pdp_type
* @param apn_name		:Access Point Name
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT+CGDCONT
*
* Return Value:
*/
int8_t type1sc_drv_operators_apn_setting(DEV_UART_PTR dev_uart_comm,AT_STRING cid, AT_STRING pdp_type, AT_STRING apn_name ,char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_WRITE, "CGDCONT","1", pdp_type, apn_name, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[Operators APN Setting]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[Operators APN Setting ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
* Type1SC driver LPWAN BAND Setting
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param band			:support band for SIM Card
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%SETCFG="BAND","band"
*
* Return Value:
*/
int8_t type1sc_drv_lpwan_band_setting(DEV_UART_PTR dev_uart_comm, AT_STRING band ,char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "SETCFG","\"BAND\"",band,NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[LPWAN BAND Setting]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[LPWAN BAND Setting ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
* TYPE1SC AT Command test
*
* @param dev_uart_com:the dev_uart_comm is serial port object
* @param recv_buf     :data buffer
* @param recv_len     :data length
*
* Return Value:
* 0,	success
* -1,	fail
*/

int8_t type1sc_drv_at_test(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT+CGPADDR
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_EXECUTE, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		//xprintf("[\"AT\" Test]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[\"AT\" Test OK]\n");
			break;
		}

		if(uart_read_cnt == 2){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;

}

/**
* TYPE1SC uart read
*
* @param dev_uart_com:the dev_uart_comm is serial port object
* @param recv_buf     :data buffer
* @param recv_len     :data length
*
* Return Value:
* 0,	success
* -1,	fail
*/
int8_t type1sc_drv_read(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len)
{

	int ret = 0;
	ret = dev_uart_comm->uart_read(recv_buf,recv_len);
//	if(0 > ret)
//	{
//		xprintf("uart read fail.\n");
//		return AT_ERROR;
//	}
	return ret;//AT_OK;
}

/**
* TYPE1SC write AT command to nbiot
*
* @param dev_uart_comm:the dev_uart_comm is serial port object
* @param mode 		  :AT_MODE
* @param command      :at command
*
* Return Value:
* -1,  fail
* else,success
*/

int8_t type1sc_drv_write_at_cmd(DEV_UART_PTR dev_uart_comm,AT_MODE mode, AT_STRING command, ...)
{
	TX_INTERRUPT_SAVE_AREA
		char at_cmd[AT_MAX_LEN] = AT_PREFIX;

		va_list vl;
		char * str = command;
		if(str == NULL){
			xprintf("[%s]%d: command is NULL, send AT test command\r\n", __FUNCTION__, __LINE__);
		} else {
			if(mode == AT_PROPRIETARY){
				strcat(at_cmd,"%");
			}else{
				strcat(at_cmd,"+");
			}

			strcat(at_cmd, command);

			switch(mode){
				case AT_LIST:
					strcat(at_cmd, "=?");
					break;
				case AT_READ:
					strcat(at_cmd, "?");
					break;
				case AT_WRITE:
					strcat(at_cmd, "=");
					va_start(vl, command);
					for(int i = 0; i < AT_MAX_PARAMETER; i++){
						str = va_arg(vl, AT_STRING);
						if(str == NULL){
							break;
						}
						if(i != 0){
							strcat(at_cmd, ",");
						}
						strcat(at_cmd, str);
					}
					va_end(vl);
					break;
				case AT_PROPRIETARY:
					strcat(at_cmd, "=");
					va_start(vl, command);
					for(int i = 0; i < AT_MAX_PARAMETER; i++){
						str = va_arg(vl, AT_STRING);
						if(str == NULL){
							break;
						}
						if(i != 0){
							strcat(at_cmd, ",");
						}
						strcat(at_cmd, str);
					}
					va_end(vl);
					break;
				case AT_EXECUTE:
				default:
					break;
			}
		}
		#ifdef AT_ADD_POSTFIX
			strcat(at_cmd, AT_POSTFIX);
		#endif /*AT_ADD_POSTFIX*/

		//xprintf("at_cmd:\"%s\" ,length:(%d)\r\n",at_cmd, strlen(at_cmd));
		int len = 0;
		len = dev_uart_comm->uart_write(at_cmd,strlen(at_cmd));
//		xprintf("\nat_cmd length:(%d),%s\r\n", len,at_cmd);
		//if( 0 > dev_uart_comm->uart_write(at_cmd,strlen(at_cmd)))
		if(0 > len)
		{
			xprintf("at cmd send fail.\n");
			return AT_ERROR;
		}

		return AT_OK;
}

/**
* Enable/Disable enter sleep mode.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param op_mode		:configuration mode
* @param value			:enable or disable
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
* - enable:
* AT%SETACFG=pm.conf.sleep_mode,enable
* - disable:
* AT%SETACFG=pm.conf.sleep_mode,disable
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/

int8_t type1sc_drv_enabnle_sleep_mode(DEV_UART_PTR dev_uart_comm, AT_STRING op_mode,AT_STRING value, char *recv_buf, uint32_t recv_len)
{

	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY,"SETACFG" ,op_mode, value, NULL))
	{
		return AT_ERROR;
	}

	return AT_OK;
}

/**
* Control light/deep sleep mode and query the sleep mode status
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param op_mode		:operation mode
* @param sleep_mode		:sleep mode
* - LS: Provides very fast entry and recovery time and is mainly used for very short sleeps. It is used for CDRX mode during the networking process.
* - DS: Provides fast recovery and entry time and is mainly used during the IDRX networking mode.
* - DH2: Provides medium entry and recovery time and is mainly used during the EDRX and IDRX networking modes.
* - DH1: Same as DH2, however IO logic is not retained.
* - DH05: Provides long entry and recovery times and is mainly used for very long inactivity intervals like PSM. The IO output values are retained in this mode.
* - DH0: Same as DH05, however IO output values are not retained.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length

* AT%SETACFG = <op_mode>,<sleep_mode>
*
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/

int8_t type1sc_drv_control_sleep_mode(DEV_UART_PTR dev_uart_comm, AT_STRING op_mode,AT_STRING sleep_mode,char *recv_buf, uint32_t recv_len)
{
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY,"SETACFG" ,op_mode, sleep_mode, NULL))
	{
		return AT_ERROR;
	}

	return AT_OK;
}


/**
* Type1SC driver configuration
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param op_mode		:operation mode
* @param value			:value
* @param recv_buf     	:data buffer
* @param recv_len     	:data length

* AT%SETCFG=<op_mode>,<value>
*
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/

int8_t type1sc_drv_setcfg(DEV_UART_PTR dev_uart_comm, AT_STRING op_mode,AT_STRING value, char *recv_buf, uint32_t recv_len)
{
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY,"SETACFG" ,op_mode, value, NULL))
	{
		return AT_ERROR;
	}

	return AT_OK;
}

/**
* Type1SC driver Enable/Disable at command echo mode
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
  -> ATE1: Enable at command echo mode
  -> ATE: Disable at command echo mode
*
* Return Value:
*/
int8_t type1sc_drv_disable_atcmd_echomode(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len)
{
#if 1
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_EXECUTE, "ATE", NULL))
	{
		return AT_ERROR;
	}
#else
	// TBD TESE 20210729
	int len = 0;
	len = dev_uart_comm->uart_write("ATE",3);
	if(0 > len)
	{
		xprintf("ATE send fail.\n");
		return AT_ERROR;
	}
#endif
	return AT_OK;
}

/**
* Type1SC driver hardware reset
*  Active: Low
* @param aIndex: GPIO pin
*
* Return Value:
*/
int8_t type1sc_drv_hw_reset(IOMUX_INDEX_E aIndex)
{
	hx_drv_iomux_set_pmux(aIndex, 3); //2:input 3:output
	hx_drv_iomux_set_outvalue(aIndex, 1);//High

	xprintf("### Type1SC HW Reset... ###\n");
	hx_drv_iomux_set_pmux(aIndex, 3);  //2:input 3:output
	hx_drv_iomux_set_outvalue(aIndex, 0);
	tx_thread_sleep(7000);//400ms
	hx_drv_iomux_set_outvalue(aIndex, 1);//High
	xprintf("### Type1SC HW Reset ok!! ###\n");

	return AT_OK;
}


/**
* Enable NBIoT Module RF
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param recv_buf     	:data buffer
* @param recv_len     	:data length

* AT+CFUN = 1
*
* Return Value:
* AT_OK, 	 success
*/
int8_t type1sc_enable_nb_rf_module(DEV_UART_PTR dev_uart_comm,char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	// AT+CFUN=1
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_WRITE, "CFUN","1", NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[AT+CFUN]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[AT+CFUN]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
* Query the device IP address
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param recv_buf     	:data buffer
* @param recv_len     	:data length

* AT+CGPADDR
*
* Return Value:
* AT_OK, 	 success
*/
int8_t type1sc_query_ip(DEV_UART_PTR dev_uart_comm,char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT+CGPADDR
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_EXECUTE, "CGPADDR", NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[IP Address]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,"+CGPADDR:")){
			//xprintf("[Get IP Address ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}else if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[Not found IP]\n*Please check environment network signal.\n*Is the SIM card inserted in the SIM card slot?\n");
#ifdef DEBUG_TEST
			break; //for test
#endif
			return AT_ERROR;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
* Query current network time
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param recv_buf     	:data buffer
* @param recv_len     	:data length

* AT+CCLK?
*
* AT+CCLK?
* +CCLK: "21/07/02,14:34:13+32"
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_query_time(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT+CCLK?
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_READ, "CCLK", NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;

		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);
#ifdef DEBUG_TEST
		recv_buf ="+CCLK: \"21/07/09,14:07:28+32\"";//for test
#endif
//		xprintf("[NetWorkTime]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,"+CCLK:")){
			//xprintf("[NetWork Time ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
* Install MQTT certification
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param operation 	    :certificate operation type
  -> ADD    - add new profile
  -> DELETE - delete profile
* @param profile_id     :numeric value to identify set of credentials used together for some TLS connection(s).
  ->Range:1~255
* @param ca_file  		:the name of the root certificate file, if it is known.
* @param ca_path 		:the path of the user-added or trusted root certificates.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%CERTCFG
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_certification_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING operation, AT_STRING profile_id, AT_STRING ca_file ,\
								   	   AT_STRING ca_path ,char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%CERTCFG
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY,"CERTCFG",operation,profile_id, \
									ca_file,ca_path,NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[NBIOT Certification]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[NBIOT Certification]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
*  MQTT event notify
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param evt_type 	    :mqtt notify events
 -> "CONCONF" - Connect procedure confirmation status
 -> "DISCONF" - Graceful disconnect procedure confirmation status
 -> "SUBCONF" - Subscribe procedure confirmation status
 -> "UNSCONF" - Unsubscribe procedure confirmation status
 -> "PUBCONF" - Outgoing publication procedure confirmation status
 -> "PUBRCV" - Incoming publication message received
 -> "CONNFAIL" - Connection failure
 -> "PUBRCVSTORFAIL" - Storage failure of received publication. Ordinary if disk out of space or file is opened for writing.
 -> "ALL" - All events, used only in execution command
* @param mode     :status of unsolicited result response presentation
  -> 0 :disable
  -> 1 :enable
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTEV
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_notify_event_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING evt_type, AT_STRING mode, char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%MQTTEV
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY,"MQTTEV", evt_type, mode, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[MQTT Events CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[MQTT Events CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
*  MQTT configure client & server nodes parameters
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cfg_type 	    :mqtt notify events
  -> "NODES" - configure client & server nodes parameters.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param client_id     	:unique client ID used to connect to the broker.
* @param domain     	:broker URL or IP address.
* @param username     	:optional username for broker authentication.
* @param password     	:optional password for broker authentication.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCFG
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_connect_service_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
										   AT_STRING client_id, AT_STRING domain, AT_STRING username, AT_STRING password, \
										   char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%MQTTCFG
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCFG", cfg_type, conn_id,\
									client_id, domain, username, password, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[MQTT NODES CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
//			xprintf("[Connect to DPS CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
*  MQTT IP layer parameters.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cfg_type 	    :mqtt notify events
  -> "IP" - configure IP layer parameters.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param pdn_identification :numeric PDN identification defined in APN table for specified PDN.
  -> 0 - use default data PDN
  -> 1-max value defined in NP config file
* @param ip_type     		:optional IP type used to configure preferred IP type for connection
  -> 0 - IPv4v6 (default)
  -> 1 - IPv4
  -> 2 - IPv6
* @param port     			:default MQTT port number is used
 -> Range - 1-65535
* @param recv_buf     		:data buffer
* @param recv_len     		:data length
*
* AT%MQTTCFG
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_ip_layer_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
								 AT_STRING pdn_identification, AT_STRING ip_type, AT_STRING port, \
								 char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%MQTTCFG
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCFG", cfg_type, conn_id,\
									pdn_identification, ip_type, port, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[MQTT IP Layer CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[MQTT IP Layer CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}


/**
*  MQTT Protocol parameters.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cfg_type 	    :mqtt notify events
  -> "PROTOCOL" - configure MQTT protocol parameters.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param protocol_type :MQTT protocol type for connection
  -> 0 - MQTT (default)
* @param keep_alive_time:The default value is 600 sec (10 min). Unit: second.
  -> 0 - no timeout, keep-alive deactivated
  -> 1 - 65535 (18 hours, 12 minutes and 15 seconds)
* @param clean_session	:clean session.
  -> 0 - the server must store the subscriptions of the client after it disconnects.
  -> 1 - the server must discard any previously maintained information about the client and treat the connection as "clean". Default policy.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCFG
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_protocol_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
								 AT_STRING protocol_type, AT_STRING keep_alive_time, AT_STRING clean_session, \
								  char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%MQTTCFG
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCFG", cfg_type, conn_id,\
									protocol_type, keep_alive_time, clean_session, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[MQTT Protocol CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[MQTT Protocol CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
*  MQTT TLS layer security parameters.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cfg_type 	    :mqtt notify events
  -> "TLS" - configure TLS layer security parameters.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param tls_authen_mode :TLS authentication mode
  -> 0 - mutual authentication (default)
  -> 1 - authenticate client side only
  -> 2 - authenticate server side only
* @param tls_pre_profile_id: TLS predefined authentication context (profile) previously configured.
  -> 10 - this parameter value fixed 10
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCFG
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_tls_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
							AT_STRING tls_authen_mode, AT_STRING tls_pre_profile_id, \
							 char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%CERTCFG
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCFG", cfg_type, conn_id,\
									tls_authen_mode, tls_pre_profile_id, NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		xprintf("[MQTT TLS CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,AT_OK_STR)){
			xprintf("[MQTT TLS CFG]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}

/**
*  MQTT Connect Server.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cfg_type 	    :mqtt notify events
  -> "CONNECT" - Start connection with endpoint.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCMD
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_connect_to_service(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id,\
									  char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;
	char *azure_connect_to_service_msgid_loc  = NULL;
	char *azure_connect_to_service_result_loc = NULL;
	char azure_connect_to_service_result[5];//1:success , 0:fail

	//AT%MQTTCMD
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCMD", cfg_type, conn_id, NULL))
	{
		return AT_ERROR;
	}

#ifdef DEBUG_TEST
	recv_buf ="%MQTTEVU:\"CONCONF\",1,0";//for test
	xprintf("\nrecv_len:\n%d %s\n",strlen(recv_buf),recv_buf); //for test
#endif
	// get at reply
	while(1){
		uart_read_cnt++;

		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);
//		xprintf("[MQTT Connect to Service]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);

		if(strstr(recv_buf,"%MQTTEVU:\"CONCONF\",1,0")!=NULL){
			xprintf("\n\nNBIOT Connect success.\n\n");
			//break;
			return AT_OK;
		}else if(strstr(recv_buf,"%MQTTEVU:\"CONCONF\",1,1")){
			xprintf("\n\nNBIOT Connect fail.\n\n");
			return AT_ERROR;
		}
		else{
			xprintf("NBIOT Connecting...\n");
		}

		if(uart_read_cnt == (UART_READ_CNT*3)){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(17500);//1s
	}//while

	return AT_OK;
}

/**
*  MQTT Subscribe to a topic on the Server.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cmd_type 	    :mqtt notify events
  ->"SUBSCRIBE" - Subscribe to a topic on the endpoint.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param qos       		:the QoS level at which the client wants to publish the message
  -> 0 - at most once delivery (default value)
  -> 1 - at least once delivery
  -> 2 - exactly once delivery
* @param subscribe_topic:the subscription topic name.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCMD
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_subscribe(DEV_UART_PTR dev_uart_comm, AT_STRING cmd_type, AT_STRING conn_id, AT_STRING qos, \
							   AT_STRING subscribe_topic, char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;

	//AT%MQTTCMD
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCMD", cmd_type, conn_id, qos, \
									subscribe_topic ,NULL))
	{
		return AT_ERROR;
	}
#ifdef DEBUG_TEST
	recv_buf = "%MQTTEVU:\"SUBCONF\",1,1,0";//for test
#endif
	// get at reply
	while(1){
		uart_read_cnt++;

		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);
//		xprintf("[NBIOT Subscribe Service]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,"%MQTTEVU:\"SUBCONF\"")){
//			xprintf("[NBIOT Subscribe Service ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}else{
			xprintf("NBIOT Subscribe ing...\n");
		}
#ifdef DEBUG_TEST
		if(strstr(recv_buf,AT_ERROR_STR)){
			xprintf("DEBUG TEST...\n[MQTT Subscribe Service]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}
#endif

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(9000);//400ms
	}//while

	return AT_OK;
}

/**
*  MQTT publish message to Server.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cmd_type 	    :mqtt notify events
  -> "PUBLISH" - Send publish packet to endpoint.
* @param conn_id        :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param qos       		:the QoS level at which the client wants to publish the message
  -> 0 - at most once delivery (default value)
  -> 1 - at least once delivery
  -> 2 - exactly once delivery
*@param will_retain	:whether or not the server will retain the message after it has been delivered to the current subscribers
 -> this is parameter fixed 0
* @param publish_topic	:the publication topic name
* @param msg_size		:actual data size in bytes for transfer to server
 -> max: 3000
* @param publish_msg	:MQTT raw data payload without quotes.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCMD
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_publish(DEV_UART_PTR dev_uart_comm, AT_STRING cmd_type, AT_STRING conn_id, AT_STRING qos, \
							AT_STRING will_retain, AT_STRING publish_topic, AT_STRING publish_msg,\
								char *recv_buf, uint32_t recv_len, uint8_t topic_type)
{
	uint8_t uart_read_cnt = 0;

	//AT%MQTTCMD
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCMD", cmd_type, conn_id, qos, \
						will_retain, publish_topic, publish_msg ,NULL))
	{
		return AT_ERROR;
	}

	// get at reply
	while(1){
		uart_read_cnt++;
		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);

		if(topic_type == PUBLISH_TOPIC_DPS_IOTHUB){
			//xprintf("[NBIOT Publish MSG DPS_IOTHUB]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			if(strstr(recv_buf,"%MQTTEVU:\"PUBRCV\"")){
				//xprintf("NBIOT Publish MSG DPS_IOTHUB ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
				break;
			}else if(strstr(recv_buf,AT_ERROR_STR)){
				//return AT_ERROR;
			}else{
				xprintf("NBIOT Publish MSG ing...\n");
			}
		}else{
			/* PUBLISH_TOPIC_SEND_DATA */
			/*
			  %MQTTCMD: 52
         	  OK
			 * */
			//xprintf("[NBIOT Publish MSG Cloud]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			if(strstr(recv_buf,AT_OK_STR)){
				//xprintf("[NBIOT Publish MSG To Cloud ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
				tx_thread_sleep(3500); //200ms
				break;
			}else if(strstr(recv_buf,AT_ERROR_STR)){
				tx_thread_sleep(14000);//800ms
				return AT_ERROR;
			}
		}

		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(9000);
	}//while

	return AT_OK;
}

/**
*  MQTT Disconnect Server.
*
* @param dev_uart_comm  :the dev_uart_comm is serial port object
* @param cfg_type 	    :mqtt notify events
  -> "DISCONNECT" - End connection with endpoint.
* @param disconn_id     :default or previously assigned <conn_id>
  -> 0 :single MQTT connectivity mode.
  -> 1 :multi-connected mode.
* @param recv_buf     	:data buffer
* @param recv_len     	:data length
*
* AT%MQTTCMD
*
* Return Value:
* AT_OK, 	 success
* AT_ERROR,  fail
*/
int8_t type1sc_MQTT_disconnect(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING disconn_id, \
								char *recv_buf, uint32_t recv_len)
{
	uint8_t uart_read_cnt = 0;
	//AT%MQTTCMD
	if(0 > type1sc_drv_write_at_cmd(dev_uart_comm, AT_PROPRIETARY, "MQTTCMD", cfg_type, disconn_id, NULL))
	{
		return AT_ERROR;
	}
	memset(recv_buf,0,AT_MAX_LEN);//clear buffer
	// get at reply
	while(1){
		uart_read_cnt++;

		type1sc_drv_read(dev_uart_comm,recv_buf,recv_len);
//		xprintf("[NBIOT MQTT Disconnect ing..]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
		if(strstr(recv_buf,"%MQTTEVU:\"DISCONF\",1,0")!=NULL){
			xprintf("[MQTT Disconnect ok!!]\nrecv_len:%d %s\n",strlen(recv_buf),recv_buf);
			break;
		}else if(strstr(recv_buf,AT_ERROR_STR)){
			tx_thread_sleep(14000);//800ms
			//break;
			return AT_ERROR;
		}else{
			xprintf("NBIOT Disconnect ing...\n");
		}
		
		if(uart_read_cnt == UART_READ_CNT){
			xprintf("uart_read timeout.\n");
			return UART_READ_TIMEOUT;
		}
		tx_thread_sleep(7000);//400ms
	}//while

	return AT_OK;
}
