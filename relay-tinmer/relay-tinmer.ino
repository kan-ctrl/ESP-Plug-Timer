#include <RtcDS1302.h>
#include <ThreeWire.h> 
// TM1637 ถูกนำออกแล้ว

// ------------------------------------
// 1. กำหนดขา DS1302 (RTC)
// ------------------------------------
#define DS1302_CLK_PIN 14 // D5 (GPIO14)
#define DS1302_DAT_PIN 12 // D6 (GPIO12)
#define DS1302_RST_PIN 13 // D7 (GPIO13)
ThreeWire myWire(DS1302_DAT_PIN, DS1302_CLK_PIN, DS1302_RST_PIN);
RtcDS1302<ThreeWire> Rtc(myWire); 

// ------------------------------------
// 2. กำหนดขา Relay 
// ------------------------------------
#define RELAY_PIN 16     // D0 (GPIO16)
const int RELAY_ON = HIGH; // สถานะ "เปิด" รีเลย์ 
const int RELAY_OFF = LOW; // สถานะ "ปิด" รีเลย์

// ตัวแปรควบคุมสถานะ
bool isRelayActive = false;
bool hasFlashed = false;


// ฟังก์ชันควบคุมรีเลย์ตามเงื่อนไข (รับค่า RtcDateTime มาทั้งก้อน)
void controlRelay(RtcDateTime now) {
    uint8_t currentHour = now.Hour();
    uint8_t currentMinute = now.Minute();
    // DayOfWeek: 0=Sunday, 1=Monday, ..., 5=Friday, 6=Saturday
    uint8_t dayOfWeek = now.DayOfWeek(); 

    bool isWeekday = (dayOfWeek >= 1 && dayOfWeek <= 5); // จันทร์-ศุกร์
    bool inActivePeriod = false;

    // ***************************************************************
    // 1. ตรวจสอบช่วงเวลาที่ 1: 05:25:00 - 05:59:59 น. (จันทร์-ศุกร์)
    // ***************************************************************
    if (isWeekday) {
        if (currentHour == 5 && currentMinute >= 25) {
            inActivePeriod = true;
        }
    } 
    
    // ***************************************************************
    // 2. ตรวจสอบช่วงเวลาที่ 2: 14:00:00 - 21:59:59 น. (ทุกวัน)
    // ***************************************************************
    if (currentHour >= 14 && currentHour < 22) {
        inActivePeriod = true;
    }

    // ---------------------------------
    // ตรรกะการสั่งงานรีเลย์ (เหมือนเดิม)
    // ---------------------------------
    if (inActivePeriod) {
        // A. ถ้าอยู่ในช่วงเวลาทำงาน
        if (!isRelayActive) {
            // ทำการกระพริบ 2 ครั้ง แล้วเปิดค้าง
            if (!hasFlashed) {
                Serial.println("Relay: กระพริบ 2 ครั้ง และเปิดค้าง...");
                // 1. กระพริบครั้งที่ 1
                digitalWrite(RELAY_PIN, RELAY_ON);
                delay(200);
                digitalWrite(RELAY_PIN, RELAY_OFF);
                delay(200);

                // 2. กระพริบครั้งที่ 2
                digitalWrite(RELAY_PIN, RELAY_ON);
                delay(200);
                digitalWrite(RELAY_PIN, RELAY_OFF);
                delay(200);

                hasFlashed = true; // ทำการกระพริบเสร็จสิ้น
            }
            
            // เปิดค้าง
            digitalWrite(RELAY_PIN, RELAY_ON);
            Serial.println("Relay: เปิดค้าง");
            isRelayActive = true;
        }
    } else {
        // B. ถ้านอกช่วงเวลาทำงาน
        if (isRelayActive) {
            // ปิดรีเลย์
            digitalWrite(RELAY_PIN, RELAY_OFF);
            Serial.println("Relay: ปิด");
            isRelayActive = false;
            hasFlashed = false; // รีเซ็ตสถานะกระพริบ
        }
    }
}


void setup () {
    Serial.begin(115200);
    while (!Serial) {} 
    
    // ตั้งค่าขา Relay
    pinMode(RELAY_PIN, OUTPUT);
    digitalWrite(RELAY_PIN, RELAY_OFF); // ปิดรีเลย์เมื่อเริ่มต้น

    Serial.println("DS1302 RTC and Relay System Start...");
    
    Rtc.Begin();
    Serial.println("--- ระบบนาฬิกาและรีเลย์ทำงาน ---");

    if (!Rtc.GetIsRunning()) {
        Serial.println("⚠️ RTC หยุดทำงาน! โปรดตั้งเวลาใหม่");
    }
}


void loop () {
    // 1. ดึงข้อมูลวันที่และเวลาจาก RTC
    RtcDateTime now = Rtc.GetDateTime();
    
    // 2. ควบคุมรีเลย์ โดยส่งค่าเวลาทั้งหมดไป
    controlRelay(now);

    // 3. แสดงผลทาง Serial Monitor (เพื่อ Debug)
    char buf[20];
    snprintf_P(buf, 
               sizeof(buf),
               PSTR("%02u/%02u/%04u %02u:%02u:%02u (Day:%u)"),
               now.Day(), now.Month(), now.Year(),
               now.Hour(), now.Minute(), now.Second(),
               now.DayOfWeek() ); // DayOfWeek 0=อาทิตย์, 1=จันทร์, ..., 6=เสาร์
    
    Serial.println(buf);
    
    // หน่วงเวลา 1 วินาที 
    delay(1000); 
}