/*****************************************************************************
* Copyright(C) 1993-2012,SZZT Corporation.
*
* File Name:   errorcode.h 
* Description: all the error code definition        
*
* Author:      liuguoxun
* Date:        2012-04-05
* History 1: 
*
*****************************************************************************/


#ifndef  _ERROR_CODE_H_20120405
#define  _ERROR_CODE_H_20120405


//base line
#define BASE_COMMON                 1000L //common for all
#define BASE_SQLITE                 1900L //SQLITE(1900~1999)

#define BASE_PINPAD                 2000L //Pinpad(2000 ~ 2099)
#define WARN_PINPAD                 2100L //Pinpad warn(2100 ~ 2199)

#define BASE_PRINTER                3000L //Printer(3000 ~ 3899)
#define WARN_PRINTER                3900L //Printer warn(3900 ~ 3999)

#define BASE_CASH                   4000L //Cash, coin and note(4000 ~ 4899)
#define WARN_CASH                   4900L //Cash, coin and note warn(4900 ~ 4999)

#define BASE_RDW                    5000L //Read & Dispenser(5000 ~ 5899)
#define WARN_RDW                    5900L //Read & Dispenser warn(5900 ~ 5999)

#define BASE_FINGER                 8000L //Finger
#define BASE_SIU                    8100L //SIU
#define BASE_CAM                    8200L //Camera
#define BASE_ALM                    8300L //Alarm
#define BASE_BAR                    8400L //Barcode

/****************************************************************************/

// common for all
#define EC_SUCCESS                                        (0L)  // 0
#define EC_OPEN_FAILED                      (BASE_COMMON +  1)  //1001 
#define EC_INVALID_PORT                     (BASE_COMMON +  2)  
#define EC_INVALID_PARA                     (BASE_COMMON +  3)  
#define EC_INVALID_DATA                     (BASE_COMMON +  4) 
#define EC_SEND_FAILED                      (BASE_COMMON +  5) 
#define EC_RECEIVED_FAILED                  (BASE_COMMON +  6)  
#define EC_USER_CANCEL                      (BASE_COMMON +  7) 
#define EC_DATA_TOOLONG                     (BASE_COMMON +  8) 
#define EC_NAK_RECEIVED                     (BASE_COMMON +  9) 
#define EC_READ_TIMEOUT                     (BASE_COMMON + 10)
#define EC_WRITE_TIMEOUT                    (BASE_COMMON + 11) 
#define EC_WAITEVENT_FAILED                 (BASE_COMMON + 12) 
#define EC_SET_FAILED                       (BASE_COMMON + 13)
#define EC_STEP_ERROR                       (BASE_COMMON + 14)
#define EC_POINTER_NULL                     (BASE_COMMON + 15)
#define EC_FULL_NOW                         (BASE_COMMON + 16)
#define EC_NET_ERROR                        (BASE_COMMON + 17)
#define EC_INVALID_FILE                     (BASE_COMMON + 18)
#define EC_TEST_MODE                        (BASE_COMMON + 19)
#define EC_EXIT                             (BASE_COMMON + 20)
#define EC_ALLOC_FAILED                     (BASE_COMMON + 21)
#define EC_TYPE_UNMATCH                     (BASE_COMMON + 22) 
#define EC_RETURN_FAILED                    (BASE_COMMON + 23) 
#define EC_SERIOUS_ERROR                    (BASE_COMMON + 24)
#define EC_UNSUPPORT                        (BASE_COMMON + 25)
#define EC_COMMAND_UNMATCH                  (BASE_COMMON + 26)
#define EC_SEQ_UNMATCH                      (BASE_COMMON + 27)
#define EC_INVALID_CLASS                    (BASE_COMMON + 28) 
#define EC_INVALID_WINDOW                   (BASE_COMMON + 29) 
#define EC_INVALID_NAME                     (BASE_COMMON + 30)
#define EC_INVALID_SIZE                     (BASE_COMMON + 31)
#define EC_INVALID_PROCESS                  (BASE_COMMON + 32)
#define EC_INVALID_THREAD                   (BASE_COMMON + 33)
#define EC_INSTANCE_FAILED                  (BASE_COMMON + 34) 
#define EC_NOT_FOUNDED                      (BASE_COMMON + 35) 


/****************************************************************************/


#define DB_SQLITE_ERROR                     (BASE_SQLITE + 1)   /* SQL error or missing database */
#define DB_SQLITE_INTERNAL                  (BASE_SQLITE + 2)   /* Internal logic error in SQLite */
#define DB_SQLITE_PERM                      (BASE_SQLITE + 3)   /* Access permission denied */
#define DB_SQLITE_ABORT                     (BASE_SQLITE + 4)   /* Callback routine requested an abort */
#define DB_SQLITE_BUSY                      (BASE_SQLITE + 5)   /* The database file is locked */
#define DB_SQLITE_LOCKED                    (BASE_SQLITE + 6)   /* A table in the database is locked */
#define DB_SQLITE_NOMEM                     (BASE_SQLITE + 7)   /* A malloc() failed */
#define DB_SQLITE_READONLY                  (BASE_SQLITE + 8)   /* Attempt to write a readonly database */
#define DB_SQLITE_INTERRUPT                 (BASE_SQLITE + 9)   /* Operation terminated by sqlite3_interrupt()*/
#define DB_SQLITE_IOERR                     (BASE_SQLITE + 10)   /* Some kind of disk I/O error occurred */
#define DB_SQLITE_CORRUPT                   (BASE_SQLITE + 11)   /* The database disk image is malformed */
#define DB_SQLITE_NOTFOUND                  (BASE_SQLITE + 12)   /* Unknown opcode in sqlite3_file_control() */
#define DB_SQLITE_FULL                      (BASE_SQLITE + 13)   /* Insertion failed because database is full */
#define DB_SQLITE_CANTOPEN                  (BASE_SQLITE + 14)   /* Unable to open the database file */
#define DB_SQLITE_PROTOCOL                  (BASE_SQLITE + 15)   /* Database lock protocol error */
#define DB_SQLITE_EMPTY                     (BASE_SQLITE + 16)   /* Database is empty */
#define DB_SQLITE_SCHEMA                    (BASE_SQLITE + 17)   /* The database schema changed */
#define DB_SQLITE_TOOBIG                    (BASE_SQLITE + 18)   /* String or BLOB exceeds size limit */
#define DB_SQLITE_CONSTRAINT                (BASE_SQLITE + 19)   /* Abort due to constraint violation */
#define DB_SQLITE_MISMATCH                  (BASE_SQLITE + 20)   /* Data type mismatch */
#define DB_SQLITE_MISUSE                    (BASE_SQLITE + 21)   /* Library used incorrectly */
#define DB_SQLITE_NOLFS                     (BASE_SQLITE + 22)   /* Uses OS features not supported on host */
#define DB_SQLITE_AUTH                      (BASE_SQLITE + 23)   /* Authorization denied */
#define DB_SQLITE_FORMAT                    (BASE_SQLITE + 24)   /* Auxiliary database format error */
#define DB_SQLITE_RANGE                     (BASE_SQLITE + 25)   /* 2nd parameter to sqlite3_bind out of range */
#define DB_SQLITE_NOTADB                    (BASE_SQLITE + 26)   /* File opened that is not a database file */


/****************************************************************************/


// Pinpad
#define	PIN_INVALID_COMMAND_PARA            (BASE_PINPAD +  1)  //2001 
#define PIN_MAC_XOR_ERROR                   (BASE_PINPAD +  2)
#define PIN_MAC_CRC_ERROR                   (BASE_PINPAD +  3)
#define PIN_MAC_KEYCOMMAND_ERROR            (BASE_PINPAD +  4)
#define PIN_INNER_ERROR                     (BASE_PINPAD +  5)
#define PIN_INVALID_DATA                    (BASE_PINPAD +  6)
#define PIN_DATA_TOOLONG                    (BASE_PINPAD +  7)
#define PIN_COMMAND_UNSUPPORT               (BASE_PINPAD +  8)
#define PIN_ALGORITHM_UNSUPPORT             (BASE_PINPAD +  9)
#define PIN_SERIAL_NUM_ERROR                (BASE_PINPAD + 10)
#define PIN_INVALID_RSA_SN                  (BASE_PINPAD + 11)
#define PIN_EPP_NOT_INITIALIZED             (BASE_PINPAD + 12)
#define PIN_SELFTEST_ERROR                  (BASE_PINPAD + 13)
#define PIN_PRESS_KEY_TIMEOUT               (BASE_PINPAD + 14)
#define PIN_KEY_UNRELEASED                  (BASE_PINPAD + 15)
#define PIN_NOPSW_OR_ERROR                  (BASE_PINPAD + 16)
#define PIN_INVALID_PIN_LENGTH              (BASE_PINPAD + 17)
#define PIN_GET_PINBLOCK_ERROR              (BASE_PINPAD + 18)
#define PIN_RANDOM_DATA_ERROR               (BASE_PINPAD + 19)
#define PIN_INVALID_ENTRY_MODE              (BASE_PINPAD + 20)
#define PIN_INVALID_WRITE_MODE              (BASE_PINPAD + 21)
#define PIN_INVALID_KEYID                   (BASE_PINPAD + 22)
#define PIN_KEY_USEVIOLATION                (BASE_PINPAD + 23)
#define PIN_KEY_NOT_LOADED                  (BASE_PINPAD + 24)
#define PIN_KEY_LOCKED                      (BASE_PINPAD + 25)
#define PIN_INVALID_MASTER_KEY              (BASE_PINPAD + 26)
#define PIN_IMK_NOT_EXIST                   (BASE_PINPAD + 27)
#define PIN_TMK_NOT_EXIST                   (BASE_PINPAD + 28)
#define PIN_KEY_NOT_EXIST                   (BASE_PINPAD + 29)
#define PIN_SAME_KEY_VALUE                  (BASE_PINPAD + 30)
#define PIN_INVALID_KEY_VALUE               (BASE_PINPAD + 31)
#define PIN_INVALID_KEY_LENGTH              (BASE_PINPAD + 32)
#define PIN_INVALID_IV_ATTRIBUTES           (BASE_PINPAD + 33)
#define PIN_INVALID_KEY_ATTRIBUTES          (BASE_PINPAD + 34)
#define PIN_INVALID_OFFSET_LENGTH           (BASE_PINPAD + 35)
#define PIN_INVALID_LENGTH_OR_SUM           (BASE_PINPAD + 36)
#define PIN_ENCRYPT_SUSPENDED               (BASE_PINPAD + 37)
#define PIN_AUTHENTICATE_LOCKED_HOURS       (BASE_PINPAD + 38)
#define PIN_COMMAND_LOCKED                  (BASE_PINPAD + 39)
#define PIN_INVALID_USERBLOCK_ADDRESS       (BASE_PINPAD + 40)
#define PIN_INVALID_MODULUS_LENGTH          (BASE_PINPAD + 41)
#define PIN_INVALID_EXPONENT_LENGTH         (BASE_PINPAD + 42)
#define PIN_INVALID_PKCS_STRUCTURE          (BASE_PINPAD + 43)
#define PIN_INVALID_PKCS_PADDING            (BASE_PINPAD + 44)
#define PIN_INVALID_SIGNATURE_LENGTH        (BASE_PINPAD + 45)
#define PIN_INVALID_SIGNATURE_SHA           (BASE_PINPAD + 46)
#define PIN_SIG_VERIFICATION_FAILED         (BASE_PINPAD + 47)
#define PIN_KCV_VERIFICATION_FAILED         (BASE_PINPAD + 48)
#define PIN_PIN_VERIFICATION_FAILED         (BASE_PINPAD + 49)
#define PIN_VERIFICATION_FAILED             (BASE_PINPAD + 50)
#define PIN_NOT_AUTHENTE                    (BASE_PINPAD + 51)
#define PIN_INVALID_AUTHENTICATION_MODE     (BASE_PINPAD + 52)
#define PIN_CERTIFICATE_NOT_EXIST           (BASE_PINPAD + 53)
#define PIN_RECV_SPECIAL_KEY                (BASE_PINPAD + 54)
#define PIN_INVALID_CERTIFICATE_FORMAT      (BASE_PINPAD + 55)
#define PIN_INVALID_CERTIFICATE_VERSION     (BASE_PINPAD + 56)
#define PIN_INVALID_CERTIFICATE_ISSUER      (BASE_PINPAD + 57)
#define PIN_INVALID_CERTIFICATE_VALIDITY    (BASE_PINPAD + 58)
#define PIN_INVALID_CERTIFICATE_SUBJECT     (BASE_PINPAD + 59)
#define PIN_INVALID_CERTIFICATE_ALGOR       (BASE_PINPAD + 60)
#define PIN_NO_CARD                         (BASE_PINPAD + 61)
#define PIN_CARD_APDU_ERROR                 (BASE_PINPAD + 62)
#define PIN_EMV_NOT_INITIALIZED             (BASE_PINPAD + 63)
#define PIN_EMV_NOT_READY                   (BASE_PINPAD + 64)
#define PIN_EMV_NEED_REINITIALIZE           (BASE_PINPAD + 65)
#define PIN_EMV_TIMEOUT                     (BASE_PINPAD + 66)
#define PIN_PSW_NOT_INITIALIZED             (BASE_PINPAD + 67)
#define PIN_EPP_NOT_INSTALLED               (BASE_PINPAD + 68)
#define PIN_INVALID_PADDING                 (BASE_PINPAD + 69)
#define PIN_PHYSICALLY_UNINSTALLED          (BASE_PINPAD + 70)
#define PIN_LOGICALLY_UNINSTALLED           (BASE_PINPAD + 71)
#define	PIN_INPUT_KEY_TIMEOUT               (BASE_PINPAD + 72)
#define	PIN_INVALID_PASSWORD_LENGTH         (BASE_PINPAD + 73)
#define	PIN_INVALID_PASSWORD                (BASE_PINPAD + 74)
#define	PIN_INPUT_PASSWORD_LOCKED           (BASE_PINPAD + 75)
#define	PIN_SYSTEM_TIME_NOT_SET             (BASE_PINPAD + 76)
#define	PIN_SYSTEM_TIME_ALREADY_SET         (BASE_PINPAD + 77)
#define	PIN_MRAM_HARDWARE_ERROR             (BASE_PINPAD + 78)
#define	PIN_DEVICE_TAMPERED                 (BASE_PINPAD + 79)
#define	PIN_SM2_ENCRYPT_FAILURE             (BASE_PINPAD + 80)
#define	PIN_SM2_DECRYPT_FAILURE             (BASE_PINPAD + 81)
#define	PIN_SM2_SIGNATURE_FAILURE           (BASE_PINPAD + 82)
#define	PIN_SM2_VERSIG_FAILURE              (BASE_PINPAD + 83)
#define	PIN_SM2_KEYEXC_FAILURE              (BASE_PINPAD + 84)
#define	PIN_SM2_VER_KEYEXC_FAILURE          (BASE_PINPAD + 85)
#define	PIN_CHIP_TIMEOUT	                (BASE_PINPAD + 86)
#define	PIN_INVALID_SM4_KEYVAL              (BASE_PINPAD + 87)
#define PIN_INVALID_INSTALLATION_MODE		(BASE_PINPAD + 88)
#define PIN_CHIP_INNER_ERROR				(BASE_PINPAD + 89)



#define PIN_EMV_ALREADY_INITIALIZED         (WARN_PINPAD +  1)  //2101
#define PIN_POWER_ERROR                     (WARN_PINPAD +  2)
#define PIN_CERTIFICATE_ALREADY             (WARN_PINPAD +  3)
#define PIN_EPP_ALREADY_INITIALIZED         (WARN_PINPAD +  4)


/****************************************************************************/

// Printer
#define	PTR_INVALID_COMMAND_PARA           (BASE_PRINTER +  1) //3001 
#define PTR_NO_PAPER                       (BASE_PRINTER +  2)
#define PTR_CUTPAPER_ERROR                 (BASE_PRINTER +  3)
#define PTR_MECHANICAL_ERROR               (BASE_PRINTER +  4)
#define PTR_INNER_ERROR                    (BASE_PRINTER +  5)

#define PTR_PAPER_LESS                     (WARN_PRINTER +  1) //3901


/****************************************************************************/

//Cash
#define	NT_TIMEOUT_CHECK_SONAR             (BASE_CASH +  1) //4001 
#define	NT_TIMEOUT_SONAR_DIVERT            (BASE_CASH +  2)
#define	NT_TIMEOUT_DIVERT_EXIT             (BASE_CASH +  3)
#define	NT_TIMEOUT_DIVERT_REJECT           (BASE_CASH +  4)
#define	NT_HAVENOTE_AT_EXIT                (BASE_CASH +  5)
#define	NT_POP_REJECTNOTE                  (BASE_CASH +  6)
#define	NT_INVALID_FLOW                    (BASE_CASH +  7)
#define	NT_INVALID_FLOW2                   (BASE_CASH +  8)
#define	NT_REJECT_REJECTNOTE               (BASE_CASH +  9)
#define	NT_CHECK_PASSAGE                   (BASE_CASH + 11)
#define	NT_ONCEOUT_TOOMUCH                 (BASE_CASH + 12)
#define	NT_REJECT_TOOMUCH                  (BASE_CASH + 13)
#define	NT_ERROR_PURGE                     (BASE_CASH + 14)
#define	NT_HAVENOTE_AT_REJECT              (BASE_CASH + 15)
#define	NT_ERROR_MOTOR_OR_SENSOR           (BASE_CASH + 17)
#define	NT_REJECTTRAY_LEAVE                (BASE_CASH + 18)
#define	NT_SENSOR_CHECK_FAILED             (BASE_CASH + 19)
#define	NT_OUTNOTE_TOOMUCH                 (BASE_CASH + 20)
#define	NT_OUTNOTE_TIMEOUT                 (BASE_CASH + 21)
#define	NT_INVALID_COMMAND                 (BASE_CASH + 22)
#define	NT_INVALID_COMMAND_PARA            (BASE_CASH + 23)
#define	NT_STEP_ERROR                      (BASE_CASH + 24)
#define	NT_WRITE_ERROR                     (BASE_CASH + 25)
#define	NT_NEED_VERIFY                     (BASE_CASH + 26)
#define	NT_WRITE_EEPROM_FAILED             (BASE_CASH + 27)
#define NT_MAC_XOR_OR_SUM_ERROR            (BASE_CASH + 28)
#define NT_SAME_CMD_SN                     (BASE_CASH + 29)
#define NT_NO_ACK_FROMAPP                  (BASE_CASH + 30)
#define NT_FINDNOTE_AT_EXIT                (BASE_CASH + 31)
#define NT_SENSOR_OPEN_DIVERT              (BASE_CASH + 32)
#define NT_SENSOR_OPEN_EXIT                (BASE_CASH + 33)
#define NT_SENSOR_OPEN_REJECT              (BASE_CASH + 34)
#define NT_SENSOR_OPEN_SONAR               (BASE_CASH + 35)
#define NT_SENSOR_CHECKFAILED_SONAR        (BASE_CASH + 36)
#define NT_SENSOR_TIMEOUT_DIVERT           (BASE_CASH + 38)
#define NT_SENSOR_CLOSE_DIVERT             (BASE_CASH + 40)
#define NT_SENSOR_CLOSE_EXIT               (BASE_CASH + 41)
#define NT_SENSOR_CLOSE_REJECT             (BASE_CASH + 42)
#define NT_SENSOR_CLOSE_SONAR              (BASE_CASH + 43)
#define NT_SENSOR_OPEN_PATH1               (BASE_CASH + 48)
#define NT_SENSOR_OPEN_CHECK1              (BASE_CASH + 49)
#define NT_SENSOR_OPEN_CSTIN1              (BASE_CASH + 50)
#define NT_SENSOR_OPEN_PATH2               (BASE_CASH + 51)
#define NT_SENSOR_OPEN_CHECK2              (BASE_CASH + 52)
#define NT_SENSOR_OPEN_CSTIN2              (BASE_CASH + 53)
#define NT_SENSOR_OPEN_PATH3               (BASE_CASH + 54)
#define NT_SENSOR_OPEN_CHECK3              (BASE_CASH + 55)
#define NT_SENSOR_OPEN_CSTIN3              (BASE_CASH + 56)
#define NT_SENSOR_OPEN_PATH4               (BASE_CASH + 57)
#define NT_SENSOR_OPEN_CHECK4              (BASE_CASH + 58)
#define NT_SENSOR_OPEN_CSTIN4              (BASE_CASH + 59)
#define NT_SENSOR_CLOSE_PATH1              (BASE_CASH + 64)
#define NT_SENSOR_CLOSE_CHECK1             (BASE_CASH + 65)
#define NT_SENSOR_CLOSE_CSTIN1             (BASE_CASH + 66)
#define NT_SENSOR_CLOSE_PATH2              (BASE_CASH + 67)
#define NT_SENSOR_CLOSE_CHECK2             (BASE_CASH + 68)
#define NT_SENSOR_CLOSE_CSTIN2             (BASE_CASH + 69)
#define NT_SENSOR_CLOSE_PATH3              (BASE_CASH + 70)
#define NT_SENSOR_CLOSE_CHECK3             (BASE_CASH + 71)
#define NT_SENSOR_CLOSE_CSTIN3             (BASE_CASH + 72)
#define NT_SENSOR_CLOSE_PATH4              (BASE_CASH + 73)
#define NT_SENSOR_CLOSE_CHECK4             (BASE_CASH + 74)
#define NT_SENSOR_CLOSE_CSTIN4             (BASE_CASH + 75)
#define NT_OUTNOTE_ERROR_BOX1              (BASE_CASH + 80)
#define NT_OUTNOTE_ERROR_BOX2              (BASE_CASH + 81)
#define NT_OUTNOTE_ERROR_BOX3              (BASE_CASH + 82)
#define NT_OUTNOTE_ERROR_BOX4              (BASE_CASH + 83)
#define NT_SENSOR_ERROR_BOX1               (BASE_CASH + 84)
#define NT_SENSOR_ERROR_BOX2               (BASE_CASH + 85)
#define NT_SENSOR_ERROR_BOX3               (BASE_CASH + 86)
#define NT_SENSOR_ERROR_BOX4               (BASE_CASH + 87)

#define NT_HAVENOTE_CHECK1                 (BASE_CASH + 88)
#define NT_HAVENOTE_CHECK2                 (BASE_CASH + 89)
#define NT_HAVENOTE_CHECK3                 (BASE_CASH + 90)
#define NT_HAVENOTE_CHECK4                 (BASE_CASH + 91)
#define NT_HAVENOTE_SONAR                  (BASE_CASH + 92)
#define NT_HAVENOTE_DIVERT                 (BASE_CASH + 93)
#define NT_HAVENOTE_EXIT                   (BASE_CASH + 94)
#define NT_HAVENOTE_REJECT                 (BASE_CASH + 95)

#define NT_OUTNOTE_FAILED_BOX1             (BASE_CASH + 96)
#define NT_OUTNOTE_FAILED_BOX2             (BASE_CASH + 97)
#define NT_OUTNOTE_FAILED_BOX3             (BASE_CASH + 98)
#define NT_OUTNOTE_FAILED_BOX4             (BASE_CASH + 99)

#define NT_MACHINE_SET_ERR				   (BASE_CASH + 100)
#define NT_UNIT_COMMUNICATION_ERR		   (BASE_CASH + 101)
#define NT_NOT_SEF_REPAIR_ERR			   (BASE_CASH + 102)
#define NT_SEF_REPAIR_ERR			       (BASE_CASH + 103)
#define NT_RETURN_DATA_ERR				   (BASE_CASH + 104)
#define NT_INVALID_SET_PARA				   (BASE_CASH + 105)

#define NT_CASHBOX_EMPTY                   (WARN_CASH +  1) //4901
#define NT_MOVE_BOX1                       (WARN_CASH + 88) //4988
#define NT_MOVE_BOX2                       (WARN_CASH + 89)
#define NT_MOVE_BOX3                       (WARN_CASH + 90)
#define NT_MOVE_BOX4                       (WARN_CASH + 91)
#define NT_EXIST_BOX1                      (WARN_CASH + 92)
#define NT_EXIST_BOX2                      (WARN_CASH + 93)
#define NT_EXIST_BOX3                      (WARN_CASH + 94)
#define NT_EXIST_BOX4                      (WARN_CASH + 95)

#define NT_CAMACCESS_HAVE_ITEMS				(WARN_CASH + 96)
#define NT_CAMSTACKER_HAVE_ITEMS			(WARN_CASH + 97)
#define NT_TSKSTACKER_HAVE_ITEMS			(WARN_CASH + 98)

#define NT_TOUCHFAILD						(WARN_CASH + 99)
 
/****************************************************************************/

//Finger
#define FINGER_OPEN_FAILED                      (BASE_FINGER +  1)  //8001 
#define FINGER_MATCH_FAILED                     (BASE_FINGER +  2)
#define FINGER_MATCH_DIFFERENT                  (BASE_FINGER +  3)
#define FINGER_MATCH_LOSS                       (BASE_FINGER +  4)
#define FINGER_VERSION_NOTSUPPORT               (BASE_FINGER +  5)
#define FINGER_CANCEL_FAILED                    (BASE_FINGER +  6)
#define FINGER_PATH_INVALID                     (BASE_FINGER +  7)
#define FINGER_MATCH_TIMEOUT                    (BASE_FINGER +  8)


/****************************************************************************/

//SIU
#define SIU_INVALID_COMMAND_PARA                (BASE_SIU +  1) //8101 
#define SIU_SYNTAX                              (BASE_SIU +  2) 
#define SIU_PORT_ERROR                          (BASE_SIU +  3) 
#define SIU_POWERSAVETOOSHORT                   (BASE_SIU +  4)


/****************************************************************************/

//Camera
#define CAM_MEDIAFULL                           (BASE_CAM +  1) //8201 
#define CAM_CAMINOP                             (BASE_CAM +  2) 
#define CAM_CHARSETNOTSUPP                      (BASE_CAM +  3) 
#define CAM_FILEIOERROR                         (BASE_CAM +  4) 
#define CAM_INVALID_COMMAND_PARA                (BASE_CAM +  5)  
#define CAM_CAMNOTSUPP                          (BASE_CAM +  6)


/****************************************************************************/


#endif //_ERROR_CODE_H_20120405

