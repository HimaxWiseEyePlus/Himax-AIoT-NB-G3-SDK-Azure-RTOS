/*
 * type1sc.h
 *
 *  Created on: 2021/7/1
 *      Author: 903990
 */

#ifndef EXTERNAL_NB_IOT_TYPE1SC_TYPE1SC_H_
#define EXTERNAL_NB_IOT_TYPE1SC_TYPE1SC_H_

#include "board.h"
#include "hx_drv_uart.h"
#include "hx_drv_iomux.h"

#ifdef NBIOT_TYPE1SC
#define AT_ADD_POSTFIX

#define AT_PREFIX 			"AT"
#define AT_POSTFIX			"\r\n"
#define AT_OK_STR			"OK"
#define AT_ERROR_STR		"ERROR"

#define AT_MAX_LEN 	    	1536
#define AT_MAX_PARAMETER 	8

#define AT_OK		     	0
#define AT_ERROR	     	-1
#define UART_READ_TIMEOUT	-2

/* Operators Low Power Wide Area Network. */
#define OPERATORS_LPWAN_TPYE LPWAN_CATM1

/* Operators Access Point Name(APN). */
#define OPERATORS_APN		 "\"internet.iot\""

/* Operators BAND. */
#define OPERATORS_BAND		 LPWAN_BAND3

/* Low Power Wide Area Network. */
#define LPWAN_CATM1			 "\"1\""
#define LPWAN_NBIOT			 "\"2\""

/* Low Power Wide Area Network Mid-BAND */
//Mid-band B1/B2/B3/B4/B25
#define LPWAN_BAND1			"\"1\""
#define LPWAN_BAND2			"\"2\""
#define LPWAN_BAND3			"\"3\""
#define LPWAN_BAND4			"\"4\""
#define LPWAN_BAND25		"\"25\""

/* Low Power Wide Area Network Low-BAND */
//Low-band B5/B8/B12/B13/B17/B18/B19/B20/B26/B28
#define LPWAN_BAND5			"\"5\""
#define LPWAN_BAND8			"\"8\""
#define LPWAN_BAND12		"\"12\""
#define LPWAN_BAND13		"\"13\""
#define LPWAN_BAND17		"\"17\""
#define LPWAN_BAND18		"\"18\""
#define LPWAN_BAND19		"\"19\""
#define LPWAN_BAND20		"\"20\""
#define LPWAN_BAND26		"\"26\""
#define LPWAN_BAND28		"\"28\""

typedef char * AT_STRING;

typedef enum {
	AT_LIST,
	AT_READ,
	AT_WRITE,
	AT_PROPRIETARY,
	AT_EXECUTE,
	AT_CMD_TEST				= 99
}AT_MODE;

typedef enum {
	PUBLISH_TOPIC_DPS_IOTHUB = 0,
	PUBLISH_TOPIC_SEND_DATA  = 1,

}PUBLISH_TOPIC_TYPE;

typedef enum {
	SEND_IMAGE_DATA 	= 0,
	SEND_CSTM_JSON_DATA = 1,
}SEND_DATA_TYPE;

typedef enum {
	NB_TYPE1SC_EN_RF_MODULE_CFG 	  	= 0,
	NB_TYPE1SC_MQTT_CERTIFICATION_CFG 	= 1,
	NB_TYPE1SC_MQTT_NOTIFY_EVENT_CFG  	= 2,
	NB_TYPE1SC_MQTT_IP_LAYPER_CFG	 	= 3,
	NB_TYPE1SC_MQTT_PROTOCOL_CFG	  	= 4,
	NB_TYPE1SC_MQTT_TLS_CFG			  	= 5,
	NB_TYPE1SC_INITIAL_CFG_DONE		  	= 6
}NB_TYPE1SC_INITIAL_CFG_STATE;

/* Operators Network Setup. */
typedef enum {
	OPERATORS_LPWAN_SETUP				= 1,
	OPERATORS_APN_SETUP					= 2,
	OPERATORS_BAND_SETUP				= 3,
	NBIOT_MODULE_RESET					= 4,
	OPERATORS_NETWORK_SETUP_DONE		= 5,
}OPERATORS_NETWORK_SETUP;



/* Driver Initial/Deinit. */
DEV_UART_PTR type1sc_drv_init(USE_SS_UART_E uart_id, uint32_t baudrate);
int8_t type1sc_drv_deinit(USE_SS_UART_E uart_id);

/* Ex-Factory setting. */
int8_t type1sc_drv_lpwan_type_sel(DEV_UART_PTR dev_uart_comm, AT_STRING lpwan_type, char *recv_buf, uint32_t recv_len);
int8_t type1sc_drv_operators_apn_setting(DEV_UART_PTR dev_uart_comm,AT_STRING cid, AT_STRING pdp_type, AT_STRING apn_name ,char *recv_buf, uint32_t recv_len);
int8_t type1sc_drv_lpwan_band_setting(DEV_UART_PTR dev_uart_comm, AT_STRING band ,char *recv_buf, uint32_t recv_len);

/* Driver control. */
int8_t type1sc_drv_at_test(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len);
int8_t type1sc_drv_read(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len);
int8_t type1sc_drv_write_at_cmd(DEV_UART_PTR dev_uart_comm,AT_MODE mode, AT_STRING command, ...);
int8_t type1sc_drv_enabnle_sleep_mode(DEV_UART_PTR dev_uart_comm, AT_STRING op_mode,AT_STRING value, char *recv_buf, uint32_t recv_len);
int8_t type1sc_drv_control_sleep_mode(DEV_UART_PTR dev_uart_comm, AT_STRING op_mode,AT_STRING sleep_mode,char *recv_buf, uint32_t recv_len);
int8_t type1sc_drv_setcfg(DEV_UART_PTR dev_uart_comm, AT_STRING op_mode,AT_STRING value, char *recv_buf, uint32_t recv_len );

/* Network info. */
int8_t type1sc_enable_nb_rf_module(DEV_UART_PTR dev_uart_comm,char *recv_buf, uint32_t recv_len);
int8_t type1sc_query_ip(DEV_UART_PTR dev_uart_comm,char *recv_buf, uint32_t recv_len);
int8_t type1sc_query_time(DEV_UART_PTR dev_uart_comm, char *recv_buf, uint32_t recv_len);

/* MQTT info. */
int8_t type1sc_MQTT_certification_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING operation, AT_STRING profile_id, AT_STRING ca_file ,\
								   	   AT_STRING ca_path ,char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_notify_event_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING evt_type, AT_STRING mode, \
									  char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_connect_service_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
										   AT_STRING client_id, AT_STRING domain, AT_STRING username, AT_STRING password, \
										   char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_ip_layer_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
								 AT_STRING pdn_identification, AT_STRING ip_type, AT_STRING port, \
								 char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_protocol_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
								 AT_STRING protocol_type, AT_STRING keep_alive_time, AT_STRING clean_session, \
								  char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_tls_cfg(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id, \
							AT_STRING tls_authen_mode, AT_STRING tls_pre_profile_id, \
							 char *recv_buf, uint32_t recv_len);


int8_t type1sc_MQTT_connect_to_service(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING conn_id,\
									   char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_subscribe(DEV_UART_PTR dev_uart_comm, AT_STRING cmd_type, AT_STRING conn_id, AT_STRING qos, \
							   AT_STRING subscribe_topic, char *recv_buf, uint32_t recv_len);

int8_t type1sc_MQTT_publish(DEV_UART_PTR dev_uart_comm, AT_STRING cmd_type, AT_STRING conn_id, AT_STRING qos, \
							AT_STRING will_retain, AT_STRING publish_topic, AT_STRING publish_msg, \
							char *recv_buf, uint32_t recv_len, uint8_t topic_type);

int8_t type1sc_MQTT_disconnect(DEV_UART_PTR dev_uart_comm, AT_STRING cfg_type, AT_STRING disconn_id, \
								char *recv_buf, uint32_t recv_len);
#endif//#ifdef NBIOT_TYPE1SC

#endif /* EXTERNAL_NB_IOT_TYPE1SC_TYPE1SC_H_ */
