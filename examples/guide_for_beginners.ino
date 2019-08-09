#include <myo.h>

armband myo;

//Edit this function with what to do on myo being connected
void connectMyo(){
  Serial.println("Connecting to Myo Armband");
  myo.connect();
  Serial.println("Connected!");


  //Below configures what the armband is doing (IMU is accelerometer + gyroscope + ... you get the idea)
  myo.set_myo_mode(myohw_emg_mode_send_emg,         //myohw_emg_mode_ + none/send_emg/send_emg_raw
                   myohw_imu_mode_none,             //myohw_imu_mode_ + none/send_data/send_events/send_all/send_raw
                   myohw_classifier_mode_enabled);  //myohw_classifier_mode_ + disabled/enabled


  //TURN_ON literally means turn on that service
  //Services available: emg/imu/battery/gesture + _notification
  //myo.<service>(TURN_ON)->registerForNotify(<function name of handler>);
  //examples:
  myo.gesture_notification(TURN_ON)->registerForNotify(showGesture);
  myo.battery_notification(TURN_ON)->registerForNotify(batLowAlarm);
  myo.emg_notification(TURN_ON)->registerForNotify(processSignal);
}


//when registerForNotify calls your handler with standard set of parameters that have to be accounted for
//Without headers, to refer to another function in a function, it has to be defined before its usage
void templateHandler(BLERemoteCharacteristic* pBLERemoteCharacteristic, uint8_t* pData, size_t length, bool isNotify){
  //pBLERemoteChar... can essentially be ignored since we aren't handling everything with one function
  //pData depends on what is being handled and can be vastly different for each service
  //length is how long pData is, after all, C++ pointer arrays easily overflow
  //isNotify can also be ignored

  //A lot of the below consists of ENUMS
  if (pData[0] == myohw_classifier_event_pose) {
    Serial.print ("Gesture: ");
    switch (pData[1]) {
      case myohw_pose_rest:
        Serial.println ("rest");
        break;
      case myohw_pose_fist:
        Serial.println ("fist");
        break;
      case myohw_pose_wave_in:
        Serial.println ("wave in");
        break;
      case myohw_pose_wave_out:
        Serial.println ("wave out");
        break;
      case myohw_pose_fingers_spread:
        Serial.println ("fingers spread");
        break;
      case myohw_pose_double_tap:
        Serial.println ("double tap");
        break;
      default:
        break;
    }
  }
  //almost ALL ENUMS are defined in myo_bluetooth.h. Refer here: github.com/thalmiclabs/myo-bluetooth
  //Use common sense & ctrl-F to find what you need.
}



void setup(){
  Serial.begin(9600);
  while(!Serial.available());
}

void loop(){
  if(!myo.connected) connectMyo(); //auto reconnect
}
