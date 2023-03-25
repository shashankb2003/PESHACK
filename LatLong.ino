#include <GNSS.h>
#include <SoftwareSerial.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <math.h>

SoftwareSerial mySerial(2, 3);  //RX, TX

#define ZSENSITIVITY 3.5
#define RZSENSITIVITY 0.2

char buff[20];

Adafruit_MPU6050 mpu;

float prevZ;
float prevRZ;

String latLon = "";

#define STRING_BUFFER_SIZE 128 /**< %Buffer size */

#define RESTART_CYCLE (60 * 5) /**< positioning test term */

static SpGnss Gnss; /**< SpGnss object */

/**
 * @enum ParamSat
 * @brief Satellite system
 */
enum ParamSat {
  eSatGps,            /**< GPS                     World wide coverage  */
  eSatGlonass,        /**< GLONASS                 World wide coverage  */
  eSatGpsSbas,        /**< GPS+SBAS                North America        */
  eSatGpsGlonass,     /**< GPS+Glonass             World wide coverage  */
  eSatGpsBeidou,      /**< GPS+BeiDou              World wide coverage  */
  eSatGpsGalileo,     /**< GPS+Galileo             World wide coverage  */
  eSatGpsQz1c,        /**< GPS+QZSS_L1CA           East Asia & Oceania  */
  eSatGpsGlonassQz1c, /**< GPS+Glonass+QZSS_L1CA   East Asia & Oceania  */
  eSatGpsBeidouQz1c,  /**< GPS+BeiDou+QZSS_L1CA    East Asia & Oceania  */
  eSatGpsGalileoQz1c, /**< GPS+Galileo+QZSS_L1CA   East Asia & Oceania  */
  eSatGpsQz1cQz1S,    /**< GPS+QZSS_L1CA+QZSS_L1S  Japan                */
};

/* Set this parameter depending on your current region. */
static enum ParamSat satType = eSatGpsGlonass;

/**
 * @brief Turn on / off the LED0 for CPU active notification.
 */
static void Led_isActive(void) {
  static int state = 1;
  if (state == 1) {
    ledOn(PIN_LED0);
    state = 0;
  } else {
    ledOff(PIN_LED0);
    state = 1;
  }
}

/**
 * @brief Turn on / off the LED1 for positioning state notification.
 *
 * @param [in] state Positioning state
 */
static void Led_isPosfix(bool state) {
  if (state) {
    ledOn(PIN_LED1);
  } else {
    ledOff(PIN_LED1);
  }
}

/**
 * @brief Turn on / off the LED3 for error notification.
 *
 * @param [in] state Error state
 */
static void Led_isError(bool state) {
  if (state) {
    ledOn(PIN_LED3);
  } else {
    ledOff(PIN_LED3);
  }
}

/**
 * @brief Activate GNSS device and start positioning.
 */
void setup() {
  /* put your setup code here, to run once: */

  int error_flag = 0;

  /* Set serial baudrate. */
  Serial.begin(115200);
  mySerial.begin(9600);

  Wire.begin();
  if (!mpu.begin(104, &Wire, 0)) {
    Serial.println("Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);

  /* Wait HW initialization done. */
  sleep(3);

  /* Turn on all LED:Setup start. */
  ledOn(PIN_LED0);
  ledOn(PIN_LED1);
  ledOn(PIN_LED2);
  ledOn(PIN_LED3);

  /* Set Debug mode to Info */
  Gnss.setDebugMode(PrintInfo);

  int result;

  /* Activate GNSS device */
  result = Gnss.begin();

  if (result != 0) {
    Serial.println("Gnss begin error!!");
    error_flag = 1;
  } else {
    /* Setup GNSS
     *  It is possible to setup up to two GNSS satellites systems.
     *  Depending on your location you can improve your accuracy by selecting different GNSS system than the GPS system.
     *  See: https://developer.sony.com/develop/spresense/developer-tools/get-started-using-nuttx/nuttx-developer-guide#_gnss
     *  for detailed information.
    */
    switch (satType) {
      case eSatGps:
        Gnss.select(GPS);
        break;

      case eSatGpsSbas:
        Gnss.select(GPS);
        Gnss.select(SBAS);
        break;

      case eSatGlonass:
        Gnss.select(GLONASS);
        break;

      case eSatGpsGlonass:
        Gnss.select(GPS);
        Gnss.select(GLONASS);
        break;

      case eSatGpsBeidou:
        Gnss.select(GPS);
        Gnss.select(BEIDOU);
        break;

      case eSatGpsGalileo:
        Gnss.select(GPS);
        Gnss.select(GALILEO);
        break;

      case eSatGpsQz1c:
        Gnss.select(GPS);
        Gnss.select(QZ_L1CA);
        break;

      case eSatGpsQz1cQz1S:
        Gnss.select(GPS);
        Gnss.select(QZ_L1CA);
        Gnss.select(QZ_L1S);
        break;

      case eSatGpsBeidouQz1c:
        Gnss.select(GPS);
        Gnss.select(BEIDOU);
        Gnss.select(QZ_L1CA);
        break;

      case eSatGpsGalileoQz1c:
        Gnss.select(GPS);
        Gnss.select(GALILEO);
        Gnss.select(QZ_L1CA);
        break;

      case eSatGpsGlonassQz1c:
      default:
        Gnss.select(GPS);
        Gnss.select(GLONASS);
        Gnss.select(QZ_L1CA);
        break;
    }

    /* Start positioning */
    result = Gnss.start(COLD_START);
    if (result != 0) {
      Serial.println("Gnss start error!!");
      error_flag = 1;
    } else {
      Serial.println("Gnss setup OK");
    }
  }

  /* Start 1PSS output to PIN_D02 */
  //Gnss.start1PPS();

  /* Turn off all LED:Setup done. */
  ledOff(PIN_LED0);
  ledOff(PIN_LED1);
  ledOff(PIN_LED2);
  ledOff(PIN_LED3);

  /* Set error LED. */
  if (error_flag == 1) {
    Led_isError(true);
    exit(0);
  }
}

/**
 * @brief %Print position information.
 */
static void print_pos(SpNavData *pNavData) {
  char StringBuffer[STRING_BUFFER_SIZE];

  // /* print time */
  // snprintf(StringBuffer, STRING_BUFFER_SIZE, "%04d/%02d/%02d ", pNavData->time.year, pNavData->time.month, pNavData->time.day);
  // Serial.print(StringBuffer);

  // snprintf(StringBuffer, STRING_BUFFER_SIZE, "%02d:%02d:%02d.%06ld, ", pNavData->time.hour, pNavData->time.minute, pNavData->time.sec, pNavData->time.usec);
  // Serial.print(StringBuffer);

  /* print satellites count */
  snprintf(StringBuffer, STRING_BUFFER_SIZE, "numSat:%2d, ", pNavData->numSatellites);
  Serial.print(StringBuffer);

  /* print position data */
  if (pNavData->posFixMode == FixInvalid) {
    Serial.print("No-Fix, ");
  } else {
    Serial.print("Fix, ");
  }
  if (pNavData->posDataExist == 0) {
    Serial.print("No Position");
  } else {
    Serial.print("");
    Serial.print(pNavData->latitude, 6);
    Serial.print(",");
    Serial.print(pNavData->longitude, 6);
    sprintf(buff, "%f,%f", pNavData->latitude, pNavData->longitude);
    latLon = String(buff);
  }

  Serial.println("");
}

/**
 * @brief %Print position information and satellite condition.
 *
 * @details When the loop count reaches the RESTART_CYCLE value, GNSS device is
 *          restarted.
 */
void loop() {
  /* put your main code here, to run repeatedly: */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);



  if (abs(prevZ - a.acceleration.z) > ZSENSITIVITY) {
    Serial.print("POTHOLEEEE  UP DOWN");
    Serial.println(latLon);
    mySerial.print(buff);
    delay(1000);
  }

  if (abs(prevRZ - g.gyro.z) > RZSENSITIVITY) {
    Serial.print("POTHOLEEEE  ROLL ROLL");
    Serial.println(latLon);
    mySerial.print(buff);
    delay(1000);
  }

  prevZ = a.acceleration.z;

  prevRZ = g.gyro.z;

  delay(50);

  static int LoopCount = 0;
  static int LastPrintMin = 0;

  /* Blink LED. */
  Led_isActive();

  /* Check update. */
  if (Gnss.waitUpdate(-1)) {
    /* Get NaviData. */
    SpNavData NavData;
    Gnss.getNavData(&NavData);

    /* Set posfix LED. */
    bool LedSet = (NavData.posDataExist && (NavData.posFixMode != FixInvalid));
    Led_isPosfix(LedSet);

    /* Print satellite information every minute. */
    if (NavData.time.minute != LastPrintMin) {
      //print_condition(&NavData);
      LastPrintMin = NavData.time.minute;
    }

    /* Print position information. */
    print_pos(&NavData);
  } else {
    /* Not update. */
    Serial.println("data not update");
  }

  /* Check loop count. */
  LoopCount++;
  if (LoopCount >= RESTART_CYCLE) {
    int error_flag = 0;

    /* Turn off LED0 */
    ledOff(PIN_LED0);

    /* Set posfix LED. */
    Led_isPosfix(false);

    /* Restart GNSS. */
    if (Gnss.stop() != 0) {
      Serial.println("Gnss stop error!!");
      error_flag = 1;
    } else if (Gnss.end() != 0) {
      Serial.println("Gnss end error!!");
      error_flag = 1;
    } else {
      Serial.println("Gnss stop OK.");
    }

    if (Gnss.begin() != 0) {
      Serial.println("Gnss begin error!!");
      error_flag = 1;
    } else if (Gnss.start(HOT_START) != 0) {
      Serial.println("Gnss start error!!");
      error_flag = 1;
    } else {
      Serial.println("Gnss restart OK.");
    }

    LoopCount = 0;

    /* Set error LED. */
    if (error_flag == 1) {
      Led_isError(true);
      exit(0);
    }
  }
}