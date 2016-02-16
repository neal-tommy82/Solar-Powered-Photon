#include "SparkFunMAX17043/SparkFunMAX17043.h" // Include the SparkFun MAX17043 library
#include "ThingSpeak/ThingSpeak.h"

TCPClient client; // TCP socket initialize

float voltage = 0; // Variable to keep track of LiPo voltage
float soc = 0; // Variable to keep track of LiPo state-of-charge (SOC)
bool alert; // Variable to keep track of whether alert has been triggered
long watchdog = 0;

unsigned long myChannelNumber = 66352;
const char * myWriteAPIKey = "XXXXXXXXXXXXXXXXXX";

void setup()
{
    lipo.begin();               // Initialize the MAX17043 LiPo fuel gauge
	lipo.quickStart();          // Quick start restarts the MAX17043 in hopes of getting a more accurate guess for the SOC.
	lipo.setThreshold(20);      // Set alert threshold to 20%.
	ThingSpeak.begin(client);	//Start the Thingspeak connection.
	delay(50);
}

void loop()
{
	    if (waitFor(WiFi.ready, 25000)) {
	    
	        if (waitFor(Particle.connected, 25000)) { //wait for 45 seconds to confirm whether the photon is connected to the cloud, if not sleep. 
        
            delay(2500); //delay to give the wifi module time to connect.
            
            voltage = lipo.getVoltage();	// returns a voltage value (e.g. 3.93)
	        soc = lipo.getSOC();  // returns the estimated state of charge (e.g. 79%)
	        //alert = lipo.getAlert();  // returns a 0 or 1 (0=alert not triggered)
            //alert = alert++;
    
            ThingSpeak.setField(2,voltage); //all fields set before one call to write data to ThingSpeak. Only one allowed per 15s.
            ThingSpeak.setField(3,soc);
            ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
            //ThingSpeak.writeField(myChannelNumber, 1, rand, myWriteAPIKey); //reference to another way to do it, but only for a single value.
    
	        System.sleep(SLEEP_MODE_DEEP,900);	    //System.sleep(600); //turns off the wifi module whilst the rest of the MCU on. Once finished it exits the loop and restarts the wifi.
            
            }
            else {
                System.sleep(SLEEP_MODE_DEEP,900);  //System sleep for 15 minutes to save power (250ma to 5ma).
                                                    //System.reset(); //just like pushing the reset button on the Photon. 
            }
	    }
        else {
            System.sleep(SLEEP_MODE_DEEP,900);
        }
}
