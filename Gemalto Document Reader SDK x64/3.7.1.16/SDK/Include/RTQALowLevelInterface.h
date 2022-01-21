/////////////////////////////////////////////////////////////////////////////
//	Low-Level QA API
/////////////////////////////////////////////////////////////////////////////

#ifndef _RTQALOWLEVELINTERFACE_H
#define _RTQALOWLEVELINTERFACE_H

#ifdef __cplusplus
extern "C" {
#endif

#include "RTQADataTypes.h"
#include "MMMReaderDataTypes.h"
#include "MMMReaderSettings.h"

//These files are included for backwards compatibility
#include "RTE8000DataTypes.h"
#include "RTE8000Settings.h"

#ifdef RTQALOWLEVELAPI_EXPORTS
#define RT_QA_LOWLEVEL_API __declspec(dllexport)
#else
#define RT_QA_LOWLEVEL_API __declspec(dllimport)
#endif


//---------------------------------------------------------------------------
//	Essential functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Initialise(
	MMMReaderErrorCallback aErrorCallback,
	void *aErrorCallbackParam
);
//Initialises the API
//
//aErrorCallback - Callback function that is called when errors occur
//aErrorCallbackParam - Pointer that will be passed to the aErrorCallback
//                      function
//
//Returns - NO_ERROR_OCCURRED if successful

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Terminate(
);
//Terminates the API
//
//Returns - NO_ERROR_OCCURRED if successful


RT_QA_LOWLEVEL_API MMMReaderHardwareConfig *RTQALL_GetHardwareConfig();

//---------------------------------------------------------------------------
//	Error functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_EnableLogging(
	bool aEnabled,
	int aLogLevel,
	int aLogMask,
	char *aFilename
);
//Turn error logging on and off
//
//aEnabled - Whether logging is turned on 
//aLogLevel - Level of logging 0 = No data 5 = Full logging data
//aLogMask - Mask to indicate which types of data to log
//aFilename - File path where the data will be saved
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_SetLoggingOptions(LoggingSettings* aOptions);
//Setup the logging strategy per settings - needs to be called after Initialization
//
//aOptions - loggings settings struct
//
//Returns - NO_ERROR_OCCURRED if successfully set

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_GetErrorMessage(
	MMMReaderErrorCode aErrorCode, 
	char *aErrorString, 
	int *aStrLen
);
//Use to obtain a text description of an error
//
//aErrorCode - Code of the error message needed
//aErrorString - Buffer to fill with the error message
//aStrLen - Size of aErrorString buffer
//
//Returns - NO_ERROR_OCCURRED If message was successfully found

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_GetLastErrorMessage(
	MMMReaderErrorCode aErrorCode, 
	char *aErrorString, 
	int *aStrLen
);
//Use to obtain a text description of an error
//
//aErrorCode - Code of the error message needed
//aErrorString - Buffer to fill with the error message
//aStrLen - Size of aErrorString buffer
//
//Returns - NO_ERROR_OCCURRED If message was successfully found

//---------------------------------------------------------------------------
//	General functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ReleaseResources(
);
//Frees up all memeory created since the previous call to this funtion. Any
//structures returned may no longer be used after this function has been
//called.
//
//Returns - NO_ERROR_OCCURRED if all memory was successfully freed.

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ReleaseResourceObject(
	void *aResourceObject
);
//Frees up a particular piece of memory, allocated and returned to the data 
//callback function. This structure may no longer be used after this function
//has been called.
//
//aResourceObject - The object to free
//
//Returns - NO_ERROR_OCCURRED if all memory was successfully freed.

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_LoadSettingsFromReader(
	MMMReaderSettings *aMMMReaderSettings
);
//Load all the settings files on the reader
//
//aMMMReaderSettings - Structure to fill with the data read from the Reader
//
//Returns - NO_ERROR_OCCURRED if files loaded successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_All_InitialiseReader(
	CameraSettings *aImageCaptureSettings, 
	char *aImageCaptureFilesDir,
	char *aIniFilesDir,
	char *aWavFilesDir,
	CameraSettings *aImageProcessingSettings,
	MMMReaderSettings *aRFIDSettings,
	MMMReaderBACCallback aOnRFIDBACStringCallback,
	MMMReaderDataCallback aOnRFIDProgressCallback,
	DirtDetectSet *aDirtDetectSettings
);
//Initialise all the different reader parts
//
//aImageCaptureSettings - The settings to use for capturing images. If NULL
//                        image capture will not be initialised
//aImageCaptureFilesDir - Directory where image capture data files are
//                        located.
//aSignalFilesDir - Directory where signal files are located. This includes
//                  sound (.wav) files and light/sound patterns.
//                  If NULL signalling will not be initialised.
//aImageProcessingSettings - The settings to use for processing images.
//                           If NULL image processing will not be intialised
//aRFIDSettings - Settings to use for RFID data capture.
//                If NULL RFID data capture will not be initialised.
//aOnRFIDBACStringCallback - Callback function used if RFID requires data to
//                           gain Basic Access Control to the RF chip.
//aOnRFIDProgressCallback - Callback function called when progress is made on
//                          RFID tasks.
//
//Returns - NO_ERROR_OCCURRED if reader successfully initialised

//RT_QA_LOWLEVEL_API BOOL RTQALL_All_IsReaderInitialised(
//);
//Specifies whether the reader has been initialised yet
//
//aForImageCapture - If the reader is ready to do image capture commands
//aForImageProcessing - If the reader is ready to do image processing
//                      commands
//aForRFID - If the reader is ready to do RFID commands
//
//Returns - TRUE if the reader has been initialised

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_All_ShutdownReader(
);
//Sends the all reader parts into pre-initialised state
//
//Returns - NO_ERROR_OCCURRED if the reader has been successfully sent to an
//          un-initialised state.

RT_QA_LOWLEVEL_API BOOL RTQALL_All_IsReaderActive(
);
//Checks whether the reader is currently carrying out any tasks
//
//Returns - TRUE if the reader is currently carrying out an image, 
//          image processing, or RFID task

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_All_CancelReaderQueue(
);
//Request that all tasks currently queued are cancelled
//
//Returns - NO_ERROR_OCCURRED if request for all tasks to be cancelled was
//          successfull


//---------------------------------------------------------------------------
//	Camera functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Camera_InitialiseReader(
	CameraSettings *aImageCaptureSettings, 
	char *aImageCaptureFilesDir
);
//Initialise all the camera part of reader
//
//aImageCaptureSettings - The settings to use for capturing images.
//aImageCaptureFilesDir - Directory where image capture data files are
//                        located.
//
//Returns - NO_ERROR_OCCURRED if reader successfully initialised

RT_QA_LOWLEVEL_API BOOL RTQALL_Camera_IsReaderInitialised(
);
//Specifies whether the camera part of the reader has been initialised yet
//
//Returns - TRUE if the camera part of the reader has been initialised

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Camera_ShutdownReader(
);
//Sends the camera part of the reader into pre-initialised state
//
//Returns - NO_ERROR_OCCURRED if the camera part of the reader has been
//          successfully sent to an un-initialised state.

RT_QA_LOWLEVEL_API BOOL RTQALL_Camera_IsReaderActive(
);
//Checks whether the camera part of the reader is currently carrying out any 
//tasks
//
//Returns - TRUE if the camera part of the reader is currently carrying out 
//          a task

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Camera_CancelReaderQueue(
);
//Request that all camera tasks currently queued are cancelled
//
//Returns - NO_ERROR_OCCURRED if request for all camera tasks to be cancelled
//          was successfull

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Camera_DetectDocOnWindowStateChange(
	bool aBlocking, 
	bool aUseUV, 
	bool aQA, 
	Box *aDocPosition,
	CameraSettings *aCamSettings, 
	OCRUserSet *aOCRUserSettings,
	DocDetectSettings *aDocDetectSettings,
	MMMReaderEventCallback aDocFoundCallback,
	MMMReaderEventCallback aDocRemovedCallback,
	void *aParam, 
	HANDLE aEvent,
	DOCPOS *aDocPosSettings, 
	IMGSET *aImgSettings
);
//Notifies if a document is place on/removed from scanner window.
//
//aBlocking - Whether to wait before returning
//aUseUV - Turn on UV lamp to detect document
//aQA -  
//aDocPosition - Gets set to the area a detected document occupies
//aCamSettings - Camera settings to use to detect documents
//aDocDetectSettings - Settings for how to detected a document
//aDocFoundCallback - Callback function to call if a document detected
//aDocRemovedCallback - Callback function to call if it is detected a 
//                      document has been removed
//aParam - Parameter passed to the callback functions aDocFoundCallback or
//         aDocRemovedCallback
//aEvent - Event to set when a doc detect has finished (including callbacks)
//aDocPosSettings -
//aImgSettings -
//
//Returns - NO_ERROR_OCCURRED If operation completed and in blocking mode, or
//          operation was launched successfully in non-blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Camera_CaptureImage(
	bool aBlocking,
	int aLight,
	BrightnessSettings *aBrightnessSettings,
	CameraSettings *aSettings,
	MMMReaderDataCallback aDataCallback,
	void *aParam, 
	int aSequenceNum,
	HANDLE aEvent
);
//Captures and returns an image of the required type
//
//aBlocking - Whether to wait for image to be taken before this function 
//            returns
//aLight - The types of light to turn on when taking the image:
//         LIGHT_RED, LIGHT_GREEN, LIGHT_BLUE, LIGHT_IRFULL, LIGHT_IRPART,
//         LIGHT_UV, LIGHT_COAXIR, LIGHT_COAX
//aBrightnessSettings - Settings to use for taking the image
//aSettings - Camera settings to use
//aDataCallback - Callback function to receive the captured image. The function
//                aDataCallback will be passed a Gdiplus::Bitmap object.
//aParam - Passed to the callback function
//aSequenceNum - Passed to the callback function
//aEvent - Event to set when the image capture process has finished
//         (including the callback)
//
//Returns - NO_ERROR_OCCURRED If operation completed and in blocking mode, or
//          operation was launched successfully in non-blocking mode


//---------------------------------------------------------------------------
//	Image Processing functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ImageProcess_InitialiseReader(
	CameraSettings *aImageProcessingSettings,
	char *aReaderDir,
	DirtDetectSet *aDirtDetectSettings,
	ImgSettings *aImgSettings
);
//Initialise all the image processing part of the reader
//
//aImageProcessingSettings - The settings to use for processing images.
//
//Returns - NO_ERROR_OCCURRED if reader successfully initialised

RT_QA_LOWLEVEL_API BOOL RTQALL_ImageProcess_IsReaderInitialised(
);
//Specifies whether the image processing part of the reader has been
//initialised yet
//
//Returns - TRUE if the image processing part of the reader has been
//          initialised

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ImageProcess_ShutdownReader(
);
//Sends the image processing part of the reader into pre-initialised state
//
//Returns - NO_ERROR_OCCURRED if the image processing part of the reader has
//          been successfully sent to an un-initialised state.

RT_QA_LOWLEVEL_API BOOL RTQALL_ImageProcess_IsReaderActive(
);
//Checks whether the image processing part of the reader is currently 
//carrying out any tasks
//
//Returns - TRUE if the image processing part of the reader is currently
//          carrying out a task

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ImageProcess_CancelReaderQueue(
);
//Request that all image processing tasks currently queued are cancelled
//
//Returns - NO_ERROR_OCCURRED if request for all image processing tasks to be
//          cancelled was successfull

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ImageProcess_ProcessImage(
	bool aBlocking, 
	Gdiplus::Bitmap *aInputImage, 
	int aLight, 
	bool aColour,
	int aOperations, 
	int aDMQuality,
	int aCorrectionMatrix,
	float aGammaValue,
	CameraSettings *aSettings, 
	DocDetectSettings *aDocDetectSettings, 
	Box *aDocPosition, 
	MMMReaderDataCallback aDataCallback,
	void *aParam, 
	int aSequenceNum, 
	HANDLE aEvent,
	DOCPOS *aDocPosSettings
);
//Process an image
//
//aBlocking - Whether to finish processing image before returning from
//            this function
//aInputImage - Image to process
//aLight - The light used to take the image 
//aColour - Return the image as colour or greyscale
//aOperations - Processing operations to carry out: 
//              PP_NORMALISE, PP_SMALL, PP_CROP, PP_ROTATE, PP_DEMOSAIC, 
//              PP_DEBARREL
//aDMQuality - The de-mosaic quality level required
//aCorrectionMatrix - The correction matrix used when converting to an image
//aSettings - Camera settings used to take the image 
//aDocPosition - The area in which the document is located
//aDataCallback - Callback function that is passed the processed image
//aParam - Parameter passed to the callback function
//aSequenceNum - Sequence number passed to the callback function
//aEvent - Event that is set when the image processing is complete
//         (including the callback function)
//aDocPosSettings - 
//
//Returns - NO_ERROR_OCCURRED If operation completed and in blocking mode, or
//          operation was launched successfully in non-blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ImageProcess_CaptureMRZData(
	bool aBlocking, 
	Gdiplus::Bitmap *aInputImage, 
	OCRSET *aOCRSettings, 
	OCRUSERSET *aOCRUserSettings,
	Box *aDocPosition, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aCallbackSequenceNum, 
	HANDLE aEvent, 
	IMGSET *aImgSettings
);
//Finds all the MRZ codelines, uses OCR to read them and carries out a series
//of measurements for each line, such as:
// left margin, right margin, top vertical offset, bottom vertical offset, 
// average stroke width of characters and codeline width.
//
//aBlocking - Whether to finish processing image before returning from
//            this function
//aInputImage - Image of the document on which to find the MRZ data
//aOCRSettings - Settings to use to carry out OCR 
//aOCRUserSettings - More settings to use when carrying out OCR
//aDocPosition - The location of the document
//aDataCallback - A callback function to receive the data collected. The
//                function aDataCallback will be passed a MRZ_DATA structure
//                containing all the data about the codeline.
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ValidateMRZCheckDigits(
	char *aMRZData, 
	int *aResult
);
//Check the check digits of an MRZ. The location and algorithms used for the
//checkdigit(s), depends on the length of the MRZ
//
//aMRZData - The MRZ data whose check digits to validate
//aResult - 1 = Valid, -1 = Invalid, 0 = Unrecognised MRZ data
//
//Returns - NO_ERROR_OCCURRED if validation was carried out without error

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_ImageConvertFormat(
	int aDestinationFormat, 
	unsigned char *aInBuffer, 
	int aInBufferLen, 
	char *aOutBuffer, 
	int *aOutBufferLen
);
//Converts an image from one format into another.
//
//aDestinationFormat - Specify which format the source image should be converted to.
//                     Should be a MMMReaderImageFormats value.
//aInBuffer - A pointer to the raw image data to convert from.
//aInBufferLen - The size of aInBuffer in bytes.
//aOutBuffer - A pointer to a writable buffer which will be updated to store the
//             converted image. This can be NULL if you wish to find out the size
//aOutBufferLen - Set this to the length of aOutBuffer. If aOutBuffer is large
//                enough to hold the converted image, it will be filled.
//                When the function returns, this value will be the actual size 
//                of the resulting image
//
//Returns - NO_ERROR_OCCURRED if validation was carried out without error


//---------------------------------------------------------------------------
//	RFID data capture functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_InitialiseReader(
	MMMReaderSettings *aRFIDSettings,
	MMMReaderBACCallback aOnRFIDBACStringCallback,
	MMMReaderDataCallback aOnRFIDProgressCallback
);
//Initialise all the RFID part of the reader
//
//aRFIDSettings - Settings to use for RFID data capture.
//aOnRFIDBACStringCallback - Callback function used if RFID requires data to
//                           gain Basic Access Control to the RF chip.
//aOnRFIDProgressCallback - Callback function called when progress is made on
//                          RFID tasks.
//
//Returns - NO_ERROR_OCCURRED if reader successfully initialised

RT_QA_LOWLEVEL_API BOOL RTQALL_RFID_IsReaderInitialised(
);
//Specifies whether the RFID part of the reader has been initialised yet
//
//Returns - TRUE if the RFID part of the reader has been initialised

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_ShutdownReader(
);
//Sends the RFID part of the reader into pre-initialised state
//
//Returns - NO_ERROR_OCCURRED if the RFID part of the reader has been
//          successfully sent to an un-initialised state.

RT_QA_LOWLEVEL_API BOOL RTQALL_RFID_IsReaderActive(
);
//Checks whether the RFID part of the reader is currently carrying out any 
//tasks
//
//Returns - TRUE if the RFID part of the reader is currently carrying out a
//          task

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_CancelReaderQueue(
);
//Request that all RFID tasks currently queued are cancelled
//
//Returns - NO_ERROR_OCCURRED if request for all RFID tasks to be cancelled
//          was successfull

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_OpenRFChip(
	bool aBlocking, 
	char *aBACString, 
	ANTENNA_MODE aAntennaMode, 
	int aMaxAPDUAttempts, 
	int aDefaultChipBaudRate, 
	bool aSelectLDSApplication,
	MMMReaderEventCallback aChipOpenedCallback, 
	void *aCallbackParam, 
	HANDLE aEvent
);
//Connect and open an RFID chip
//
//aBlocking - Whether to wait for chip to be opened before returning 
//aChipId - The ID of the chip to open. If blank the first chip found is
//          opened
//aBACString - Data used to gain Basic Access Control to the chip if it
//             uses it. 
//             If blank and a BAC string is needed, the function provided in
//             the parameter aOnRFIDBACStringCallback of the 
//             RTQALL_InitialiseReader() function will be called.
//aChipOpenedCallback - Function called when the data has chip has been
//                      opened
//aCallbackParam - Param passed to the callback function
//aEvent - Event set when the process has finished (including the callback)
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_GetRFFile(
	bool aBlocking, 
	MMMReaderRFItem aItem,
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
// Retrieve the requested data file from the RF reader, as the raw binary file.
//
//aBlocking - Whether to wait for the data to be read before returning
//aItem - The file item to get: RFID_EF_COM, RFID_EF_SOD, RFID_DG1, RFID_DG2,
//        RFID_DG3, RFID_DG4, RFID_DG5, RFID_DG6, RFID_DG7, RFID_DG8,
//        RFID_DG9, RFID_DG10, RFID_DG11, RFID_DG12, RFID_DG13, RFID_DG14,
//        RFID_DG15, RFID_DG16
//aDataCallback - Callback function to pass the data read to
//aCallbackParam - Parameter passed to the callback function
//aSequenceNum - SequenceNum paramteter passed to callback function
//aEvent - Event that is set when the process has completed 
//         (including callback)
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_ValidateRFDataGroup(
	bool aBlocking, 
	MMMReaderRFItem aItem, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
// Determine whether the hash of a DataGroup is valid
//
//aBlocking - Whether to wait for the data to be vaidated before returning
//aItem - The file item to validate: RFID_DG1, RFID_DG2, RFID_DG3, RFID_DG4,
//        RFID_DG5, RFID_DG6, RFID_DG7, RFID_DG8, RFID_DG9, RFID_DG10,
//        RFID_DG11, RFID_DG12, RFID_DG13, RFID_DG14, RFID_DG15, RFID_DG16
//aDataCallback - Callback function for the validation result to be sent to 
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_DecodeRFDataGroup(
	bool aBlocking, 
	MMMReaderRFItem aItem,
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
//Get the decoded contents of a DataGroup
//
//aBlocking - Whether to wait for the data to be decoded before returning
//aItem - The file item to validate: RFID_DG1, RFID_DG2
//aDataCallback - Callback function for the decoded data to be sent to 
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_ValidateRFSignature(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
//Validate the signature on the RF chip
//
//aBlocking - Whether to wait for the signature to be validated before
//            returning
//aDataCallback - Callback function for the valdiation result to be sent to
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_ValidateRFSignedAttributes(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
//Validate the signed attributes on the RF chip
//
//aBlocking - Whether to wait for the signed attributes to be validated
//            before returning
//aDataCallback - Callback function for the valdiation result to be sent to
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed/was launched successfully
//                            depending on blocking mode

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_GetRFAirBaudRate(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
//Get the air baud rate that is currently being used
//
//aBlocking - Whether to wait for the before returning
//aDataCallback - Callback function for the result to be sent to
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_GetRFChipId(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
//Get the chip id of the chip being read
//
//aBlocking - Whether to wait for the before returning
//aDataCallback - Callback function for the result to be sent to
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_RFPowerOff(
	bool aBlocking, 
	MMMReaderEventCallback aEventCallback, 
	void *aCallbackParam, 
	HANDLE aEvent
);

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_GetRFBACStatus(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);
//Determine whether Basic Access Control has been used to open the chip.
//NB. RTQALL_DecodeRFDataGroup() or RTQALL_DecodeRFDataGroup() must have been
//    called, for this chip, prior to this function. As there is no way of 
//    knowing whether BAC is needed until you actually read some data from
//    a chip.
//
//aBlocking - Whether to wait for the before returning
//aDataCallback - Callback function for the result to be sent to
//aCallbackParam - Parameter sent to the callback function
//aSequenceNum - SequenceNum parameter sent to the callback function 
//aEvent - Even that is set when the process has completed (including the 
//         callback function)
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_GetChipAuthStatus(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_GetTerminalAuthStatus(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);

RT_QA_LOWLEVEL_API MMMReaderErrorCode RFID_CheckActiveAuthentication(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);

RT_QA_LOWLEVEL_API MMMReaderErrorCode RFID_ValidateDocSignerCert(
	bool aBlocking, 
	MMMReaderDataCallback aDataCallback, 
	void *aCallbackParam, 
	int aSequenceNum, 
	HANDLE aEvent
);

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_SendAPDU(
	bool aBlocking, 
	unsigned char* aAPDU, 
	int aAPDULen,
	unsigned char* aResponseBuffer, 
	int *aResponseBufferLen,
	HANDLE aEvent
);

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_ResetRFPassport();
//Resets internal RF data so that all data (and BAC) will be read again
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_RFID_UpdateSettings(RFProcessSettings *aRFSettings);
//used to update settings such as certificates dir, and read settings such as baud rate, retry count etc

//---------------------------------------------------------------------------
//	Signal (LED/Sound) functions
//---------------------------------------------------------------------------

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Signal_InitialiseReader(
	char *aIniFilesDir, char *wavFilesDir
);
//Initialise all the signalling part of the reader
//
//aSignalFilesDir - Directory where signal files are located. This includes
//                  sound (.wav) files and light/sound patterns.
//
//Returns - NO_ERROR_OCCURRED if signal part of the reader was successfully
//          initialised

RT_QA_LOWLEVEL_API BOOL RTQALL_Signal_IsReaderInitialised(
);
//Specifies whether the signal part of the reader has been initialised yet
//
//Returns - TRUE if the signal part of the reader has been initialised

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Signal_ShutdownReader(
);
//Sends the signal part of the reader into pre-initialised state
//
//Returns - NO_ERROR_OCCURRED if the signal part of the reader has been
//          successfully sent to an un-initialised state.

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Signal_SignalEvent(
	SoundSettings *aSoundSettings,
	LedSettings *aLedSettings,
	MMMReaderSignal aEvent
);
//Indicates both with the leds and a sound that some event has occurred
//
//aEvent - Event to signal with LEDs and/or speaker
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Signal_LedEvent(
	LedSettings *aLedSettings,
	MMMReaderSignal aEvent
);
// Indicates with the leds that some event has occurred
//
//aEvent - Event to signal with LEDs
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

RT_QA_LOWLEVEL_API MMMReaderErrorCode RTQALL_Signal_SoundEvent(
	SoundSettings *aSoundSettings,
	MMMReaderSignal aEvent
);
// Indicates with a sound that some event has occurred
//
//aEvent - Event to signal with speaker
//
//Returns - NO_ERROR_OCCURRED If operation completed successfully

#ifdef __cplusplus
 } 
#endif

#endif