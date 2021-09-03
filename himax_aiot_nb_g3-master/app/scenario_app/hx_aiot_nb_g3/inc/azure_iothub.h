/*
 * azure_iothub.h
 *
 *  Created on: 2021¦~4¤ë18¤é
 *      Author: 903990
 */

#ifndef SCENARIO_APP_INC_AZ_IOTHUB_INC_AZURE_IOTHUB_H_
#define SCENARIO_APP_INC_AZ_IOTHUB_INC_AZURE_IOTHUB_H_

#include <hx_aiot_nb_g3/inc/bodydetect_meta.h>
#include "tx_api.h"
#include "nx_azure_iot_hub_client.h"
#include "board.h"

#ifdef __cplusplus
extern   "C" {
#endif

/*########################################################################################################################*/
/**********************************************Define Connect Mode*********************************************************/
/*########################################################################################################################*/
/* Defined, standalone Test*
 * 0: Connect to Azure Central Cloud
 * 1: Standalone , Azure Portal new DPS and IOTHUB
 * */
#define AZURE_DPS_IOTHUB_STANDALONE_TEST		1

/* Enable IoT Plug and Play Certificaiton. */
//#define AZURE_PNP_CERTIFICATION_EN

#if AZURE_DPS_IOTHUB_STANDALONE_TEST
/* Defined, Azure Portal create iothub connect. */
/* https://portal.azure.com */
#define ENABLE_AZURE_PORTAL_DPS_HUB_DEVICE
#else
/* Defined, Azure Central connect to cloud. */
/* https://portal.azure.com */
#define ENABLE_AZURE_CENTRAL_DPS_HUB_CLOUD

#endif /* DPS_STANDALONE_TEST */


//#define MODEL_ID_WEIPLUS_V1
#define MODEL_ID_WEIPLUS_V2
//#define MODEL_ID_HIMAX_AIOT_NB_G2_V2
/*########################################################################################################################*/

/*########################################################################################################################*/
/*************************************************AZURE_PORTAL_DPS_HUB_DEVICE**********************************************/
/*########################################################################################################################*/

#ifdef ENABLE_AZURE_PORTAL_DPS_HUB_DEVICE
/* Required when DPS is used.  */
#ifndef ENDPOINT
#define ENDPOINT                                   "global.azure-devices-provisioning.net"
#endif /* ENDPOINT */

/* AZURE_PNP_CERTIFICATION HOST NAME auto get in azure_pnp_iotdps_get_registration_status() */
#ifndef AZURE_PNP_CERTIFICATION_EN
#ifndef HOST_NAME
/* Azure Portal IoTHub name */
/* Key-in your IOTHUB Name. Ex:"xxxx..azure-devices.net" */
#define HOST_NAME                                   "WeiPlusIoTHub.azure-devices.net"
#endif /* HOST_NAME */
#endif /*AZURE_PNP_CERTIFICATION_EN */

#ifndef ID_SCOPE
/* Azure Portal scope id. */
/* Key-in your scope id. Ex:"0nxxxxD36F3" */
#ifndef AZURE_PNP_CERTIFICATION_EN
#define ID_SCOPE                                   "0ne001D36F3"
#else
#define ID_SCOPE                                   "0ne000FFA42"
#endif
#endif /* ID_SCOPE */

#ifndef DEVICE_ID
/* Azure Portal device id. */
/* Key-in your scope id. Ex:"weiplus01" */
#ifndef AZURE_PNP_CERTIFICATION_EN
#define DEVICE_ID                                  "weiplus01"
#else
#define DEVICE_ID                                  "0d137b65-0ddf-4e23-bd7c-d4c94df8b66d"
#endif
#endif /* DEVICE_ID */

#ifndef REGISTRATION_ID
#ifndef AZURE_PNP_CERTIFICATION_EN
/* Azure Portal registration id .*/
/* Key-in your registration id. Ex:"weiplus" */
#define REGISTRATION_ID                             "weiplus"
#else
#define REGISTRATION_ID                             DEVICE_ID
#endif
#endif /* REGISTRATION_ID */

#ifndef DEVICE_SYMMETRIC_KEY
/* SYMMETRIC KEY.  */
/* Key-in your symmetric key. */
#ifndef AZURE_PNP_CERTIFICATION_EN
#define DEVICE_SYMMETRIC_KEY               			"yuXAeCiwD5/R6/H/0fH3C88SEcX40zKafS83EK3Np8epLQ9ZWKT1uD5+DaBspQqBMrHWnx6PZU/0kR6qd/TLqw=="
#else
#define DEVICE_SYMMETRIC_KEY               			"KeO10cEbecr1oHb5nA9S129TjkHJlAr2eL9CvClcXx8="
#endif
#endif /* DEVICE_SYMMETRIC_KEY */

#endif /* ENABLE_AZURE_PORTAL_DPS_HUB_DEVICE */
/*########################################################################################################################*/

/*########################################################################################################################*/
/************************************************AZURE_CENTRAL_DPS_HUB_CLOUD***********************************************/
/*########################################################################################################################*/

#ifdef ENABLE_AZURE_CENTRAL_DPS_HUB_CLOUD

/* Required when DPS is used.  */
#ifndef ENDPOINT
#define ENDPOINT                                   "global.azure-devices-provisioning.net"
#endif /* ENDPOINT */


#ifndef REGISTRATION_ID
/* Azure Central registration id(Device_ID) .*/
/* Key-in your device id. Ex:"weiplus01" */
#define REGISTRATION_ID                             "weiplus01"
#endif /* REGISTRATION_ID */

#ifndef ID_SCOPE
/* Azure Central scope id. */
/* Key-in your scope id. Ex:"0nxxxx14C2E" */
#define ID_SCOPE                                    "0ne00214C2E"
#endif /* ID_SCOPE */

#ifndef DEVICE_SYMMETRIC_KEY
/* SYMMETRIC KEY.  */
/* Key-in your symmetric key. */
//#define DEVICE_SYMMETRIC_KEY						"+2IiIRPz/opdQYm8RoyI4eTOfsqkJklpg1fvLIBj+0GuiMUzKLPIQjOurjCtX0tYJv+7QnsGCDQcJkBUWd6+pg=="
#define DEVICE_SYMMETRIC_KEY						"p6tbQ7hR5LQDVDXf9R4EcaUVEemnG0EOr28xSCX345A="

#endif /* DEVICE_SYMMETRIC_KEY */

#endif /* ENABLE_AZURE_CENTRAL_DPS_HUB_CLOUD */
/*########################################################################################################################*/

/*########################################################################################################################*/
/**********************************************Define MQTT Connect Info****************************************************/
/*########################################################################################################################*/
/*  Model ID.  */
#ifndef MODEL_ID
#ifdef MODEL_ID_WEIPLUS_V1
/* https://github.com/Azure/iot-plugandplay-models/blob/main/dtmi/himax/weiplus-1.json */
#define MODEL_ID                                   				"dtmi:himax:weiplus;1"
#elif defined(MODEL_ID_WEIPLUS_V2)
/* https://github.com/Azure/iot-plugandplay-models/blob/main/dtmi/himax/weiplus-2.json */
#define MODEL_ID                                   				"dtmi:himax:weiplus;2" // dtmi:realtek:rtk_ameba_series;1
#elif defined(MODEL_ID_HIMAX_AIOT_NB_G2_V2)
/* https://github.com/HimaxSPD/iot-plugandplay-models/blob/main/dtmi/himax/himax_aiot_nb_g2-2.json */
#define MODEL_ID                                   				"dtmi:himax:himax_aiot_nb_g2;2"
#endif
#endif /* MODEL_ID */


/* Azure IoT DPS subscribe topic.  */
#ifndef AZURE_IOTDPS_CLIENT_REGISTER_SUBSCRIBE_TOPIC
#define AZURE_IOTDPS_CLIENT_REGISTER_SUBSCRIBE_TOPIC 			"$dps/registrations/res/#"
#endif


/* Azure IoT DPS register publish topic.  */
#ifndef AZURE_IOTDPS_CLIENT_REGISTER_PUBLISH_TOPIC
#define AZURE_IOTDPS_CLIENT_REGISTER_PUBLISH_TOPIC 				"$dps/registrations/PUT/iotdps-register/?$rid=1"
#endif


/* Azure IoT DPS get register publish topic.  */
#ifndef AZURE_IOTDPS_GET_CLIENT_REGISTER_STATUS_PUBLISH_TOPIC
#define AZURE_IOTDPS_GET_CLIENT_REGISTER_STATUS_PUBLISH_TOPIC 	"$dps/registrations/GET/iotdps-get-operationstatus/?$rid=1&operationId="
#endif

/* Azure IoT DPS Service version.  */
#ifndef AZURE_IOTDPS_SERVICE_VERSION
#define AZURE_IOTDPS_SERVICE_VERSION 							"2019-03-31"
#endif

/* Define the default MQTT TLS (secure) port number */
#ifndef NBIOT_MQTT_TLS_PORT
#define NBIOT_MQTT_TLS_PORT                                     "8883"
#endif

/* Azure IoT DPS Service version.  */
#ifndef AZURE_IOTHUB_SERVICE_VERSION
#define AZURE_IOTHUB_SERVICE_VERSION 							"2020-09-30"
#endif

/* Azure IoTHub Device Name */
#ifndef AZURE_IOTHUB_DEVICE_ID
#ifdef ENABLE_AZURE_PORTAL_DPS_HUB_DEVICE
#define AZURE_IOTHUB_DEVICE_ID                                   DEVICE_ID
#else
#define AZURE_IOTHUB_DEVICE_ID                                   REGISTRATION_ID
#endif
#endif /* AZURE_IOTHUB_DEVICE_ID */

#ifdef MODEL_ID_WEIPLUS_V1
/* model id weiplus-1 */
/* Azure IoTHub publish message for json format  */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX              "{\"human\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX              "}"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX */
#elif defined(MODEL_ID_WEIPLUS_V2)
/* model id weiplus-2 */
/* Azure IoTHub publish message for json format  */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX              		"{\"human\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX */


#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_X
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_X           		",\"det_box_x\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_X */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_Y
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_Y           		",\"det_box_y\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_Y */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_WIDTH
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_WIDTH           	",\"det_box_width\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_WIDTH */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_HEIGHT
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_HEIGHT           	",\"det_box_height\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_HEIGHT */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX              		"}"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX */
#elif defined(MODEL_ID_HIMAX_AIOT_NB_G2_V2)
/* model id himax_aiot_nb_g2-2 */
/* Azure IoTHub publish message for json format  */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX              		"{\"human\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_PREFIX */


#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_X
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_X           		",\"det_box_x\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_X */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_Y
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_Y           		",\"det_box_y\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_Y */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_WIDTH
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_WIDTH           	",\"det_box_width\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_WIDTH */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_HEIGHT
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_HEIGHT           	",\"det_box_height\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_DET_BOX_HEIGHT */

/* INAGE */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_IMAGE
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_IMAGE           			",\"image\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_IMAGE */

/* AUDIO */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_AUDIO
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_AUDIO           			",\"audio\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_AUDIO */

/* ACCELERMETERS */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_X
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_X           		",\"accel_x\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_X */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_Y
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_Y           		",\"accel_y\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_Y */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_Z
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_Z           		",\"accel_z\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_ACCEL_Z */

/* GYRO */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_X
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_X           			",\"gyro_x\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_X */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_Y
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_Y           			",\"gyro_y\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_Y */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_Z
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_Z           			",\"gyro_z\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_GYRO_Z */

/* MAGNETOMETER */
#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_X
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_X           			",\"mag_x\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_X */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_Y
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_Y           			",\"mag_y\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_Y */

#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_Z
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_Z           			",\"mag_z\":"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_MAG_Z */


#ifndef AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX
#define AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX              		"}"
#endif /* AZURE_IOTHUB_PUBLISH_MESSAGE_JSON_SUFFIX */

#endif


/*########################################################################################################################*/

/*########################################################################################################################*/
/*************************************************Azure DPS Connect Event**************************************************/
/*########################################################################################################################*/
/* Azure Plug and Play Device Provisioning Service(DPS) Event. */
typedef enum {
	PNP_IOTDPS_INITIAL						= 0,
	PNP_IOTDPS_GET_NETWORK_TIME				= 1,
	//PNP_IOTDPS_NBIOT_CERTIFICATION		= 2,
	PNP_IOTDPS_CONNECT_TO_DPS_CFG			= 3,
	PNP_IOTDPS_CONNECT_TO_DPS				= 4,
	PNP_IOTDPS_REGISTRATION_SUBSCRIBE		= 5,
	PNP_IOTDPS_REGISTRATION_PUBLISH			= 6,
	PNP_IOTDPS_GET_REGISTRATION_OPERATION_ID= 7,
	PNP_IOTDPS_GET_REGISTRATION_STATUS		= 8,
	PNP_IOTDPS_REGISTRATION_DONE			= 9,
	PNP_IOTDPS_RECONNECT					= 10,
	PNP_IOTDPS_DISCONNECT
}AZURE_PNP_IOTDPS_EVENT;


/*########################################################################################################################*/
/************************************************Azure IOTHUB Connect Event************************************************/
/*########################################################################################################################*/
/* Azure Plug and Play IOTHUB Event. */
typedef enum {
	PNP_IOTHUB_INITIAL						= 0,
	PNP_IOTHUB_NBIOT_IDLE					= 1,
	PNP_IOTDPS_CONNECT_TO_DEVICE_CFG		= 2,
	PNP_IOTHUB_CONNECT_TO_DEVICE			= 3,
	PNP_IOTHUB_MESSAGE_PUBLISH				= 4,
	PNP_IOTHUB_CONNECT_TO_DEVICE_DONE		= 5,
	PNP_IOTHUB_CONNECTIING					= 6,
	PNP_IOTHUB_RECONNECT
}AZURE_PNP_IOTHUB_EVENT;


/*########################################################################################################################*/
/****************************************************Algorithm Event*******************************************************/
/*########################################################################################################################*/
/* Azure Algorithm Event. */
typedef enum {
	ALGO_EVENT_IDLE							= 0,
	ALGO_EVENT_SEND_RESULT_TO_CLOUD			= 1,
	ALGO_EVENT_SEND_IMAGE_TO_CLOUD			= 2,
	ALGO_EVENT_SEND_RESULT_AND_IMAGE		= 3,
	ALGO_EVENT_IOTHUB_RECONNECT				= 4,
	ENTER_PMU_MODE							= 5
}AZURE_ALGORITHM_EVENT;

#ifdef MODEL_ID_HIMAX_AIOT_NB_G2_V2
/* Accelerometers structure. */
typedef struct
{
	double accel_x;
	double accel_y;
	double accel_z;
}struct_AccelerometersResult;

/* Gyro structure. */
typedef struct
{
	double gyro_x;
	double gyro_y;
	double gyro_z;
}struct_GyroResult;

/* Magnetometer structure. */
typedef struct
{
	double mag_x;
	double mag_y;
	double mag_z;
}struct_MagnetometerResult;

extern struct_AccelerometersResult	accel_result;
extern struct_GyroResult			gyro_result;
extern struct_MagnetometerResult 	mag_result;

#endif /* MODEL_ID_HIMAX_AIOT_NB_G2_V2 */

/* Azure Algorithm Event. */
extern uint8_t azure_active_event;


/*
 * Send Algorithm Metadata to Cloud.
 * */
int8_t send_algo_result_to_cloud();

/*
 * Send custom json data or image to cloud.
 * */
int8_t send_cstm_data_to_cloud(unsigned char *databuf, int size, uint8_t send_type);

#if (USE_DEVICE_CERTIFICATE == 1)

/* Using X509 certificate authenticate to connect to IoT Hub,
   set the device certificate as your device.  */

/* Device Key type. */
#ifndef DEVICE_KEY_TYPE
#define DEVICE_KEY_TYPE                             NX_SECURE_X509_KEY_TYPE_RSA_PKCS1_DER
#endif /* DEVICE_KEY_TYPE */

#endif /* USE_DEVICE_CERTIFICATE */

/*
END TODO section
*/

/* Define the Azure RTOS IOT thread stack and priority.  */
#ifndef NX_AZURE_IOT_STACK_SIZE
#define NX_AZURE_IOT_STACK_SIZE                     (2048)
#endif /* NX_AZURE_IOT_STACK_SIZE */

#ifndef NX_AZURE_IOT_THREAD_PRIORITY
#define NX_AZURE_IOT_THREAD_PRIORITY                (4)
#endif /* NX_AZURE_IOT_THREAD_PRIORITY */

#ifndef SAMPLE_MAX_BUFFER
#define SAMPLE_MAX_BUFFER                           (256)
#endif /* SAMPLE_MAX_BUFFER */

/* Define the sample thread stack and priority.  */
#ifndef SAMPLE_STACK_SIZE
#define SAMPLE_STACK_SIZE                           (2048)
#endif /* SAMPLE_STACK_SIZE */

#ifndef SAMPLE_THREAD_PRIORITY
#define SAMPLE_THREAD_PRIORITY                      (16)
#endif /* SAMPLE_THREAD_PRIORITY */

/* Define sample properties count.  */
#define MAX_PROPERTY_COUNT                          2

/* Define Sample context.  */
typedef struct SAMPLE_CONTEXT_STRUCT
{
    UINT                                state;
    UINT                                action_result;
    ULONG                               last_periodic_action_tick;

    TX_EVENT_FLAGS_GROUP                sample_events;

    /* Generally, IoTHub Client and DPS Client do not run at the same time, user can use union as below to
       share the memory between IoTHub Client and DPS Client.

       NOTE: If user can not make sure sharing memory is safe, IoTHub Client and DPS Client must be defined seperately.  */
    union SAMPLE_CLIENT_UNION
    {
        NX_AZURE_IOT_HUB_CLIENT             iothub_client;
#ifdef ENABLE_DPS_SAMPLE
        NX_AZURE_IOT_PROVISIONING_CLIENT    prov_client;
#endif /* ENABLE_DPS_SAMPLE */
    } client;

#define iothub_client client.iothub_client
#ifdef ENABLE_DPS_SAMPLE
#define prov_client client.prov_client
#endif /* ENABLE_DPS_SAMPLE */

} SAMPLE_CONTEXT;

#ifdef __cplusplus
}
#endif

#endif /* SCENARIO_APP_INC_AZ_IOTHUB_INC_AZURE_IOTHUB_H_ */
