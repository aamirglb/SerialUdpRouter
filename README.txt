/*************************************************************/
/* README file for Mx10 Simulator Version 0.1                */
/*************************************************************/

1) Currently the simulator allow sending following Mx10 Status messages
   Command#1:  Gimbal Status Response (Allow setting only Gimbal Mode)
   Command#25: Active Errors Status

2) Simulator is not poll based like actual Mx10 System.
   Once serial communication is started by selecting option 5, it continuously 
   send status message.
   
2) The simulator perform very minimal error handling.

3) The simulator doesn't support control character escaping at HDLC layer.

4) The simulator uses CTB library for serial communication and Boost library
   for threading.
   
 
/*************************************************************/
/* README file for Mx10 Simulator Version 0.2                */
/*************************************************************/  

1) Added circular buffer class for processing of received packets.

2) Add polling functionality. Now simulator wait for a command to send status back.
   Still doesn't check acknowledgment bit, it always sends a command back.
   
3) Simulator support the following payload commands
   Command#1:  Gimbal Status Command
   Command#10: EO/EOW Sensor Control Command 
   Command#25: Active Errors Status
   
4) Now simulator provides option to set Gimbal Position (Azimuth/Elevation)
   (doesn't validate the values) and Horizontal Field of View.
   
/*************************************************************/
/* README file for Mx10 Simulator Version 0.3                */
/*************************************************************/  

1) Added horizontal slewing of payload.

2) Cleanup compiler warnings.              

/*************************************************************/
/* README file for Mx10 Simulator Version 0.4                */
/*************************************************************/  

1) Added HDLC transparency for received and transimitted packets.

2) Added processing of Gimbal Control command packet.


/*************************************************************/
/* README file for Mx10 Simulator Version 0.5                */
/*************************************************************/  

1) Added slew of payload from external source (SkyView GCS).

2) Added setting Target position and range.

/*************************************************************/
/* README file for Mx10 Simulator Version 0.6                */
/*************************************************************/  

1) Moved the Mx10 Alert related code into a separate source file (Mx10Error.cpp).

2) Added Zoom In & Zoom Out option from simulator.

/*************************************************************/
/* README file for Mx10 Simulator Version 0.7                */
/*************************************************************/ 
1) Added comments for few functions.
2) Added commanding of HFoV from external source.

/*************************************************************/
/* README file for Mx10 Simulator Version 0.10                */
/*************************************************************/ 
1) Added FOV for IR sensor.
2) Added user input validation to avoid infinite loops.