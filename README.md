# ESP-Plug-Timer
I created this ESP-Plug-Timer to time the lights in my aquarium, so the code will make it blink when turned on to select the color mode of the aquarium lights.
Components: ESP8266 , relay , power strip , rtc ds1302 , AC-to-DC converter
Make sure the voltage used to drive the relay from the ESP side is compatible.
VeAnd make sure the relay can handle the maximum current that the load will draw.
The AC-to-DC converter I use has a 220 VAC input and a 5 VDC, 700 mA output. I use two of them. Because using only one doesn't work properly.
The results were as expected. It works, but security concerns remain. Judging from the design, it doesn't look secure at all. Anyone who tries to follow it will have to accept the risks.
The advantage is that the ESP8266 supports Wi-Fi, so you can modify it in many ways or even make it IoT.
<img width="1381" height="791" alt="5v" src="https://github.com/user-attachments/assets/bb02ba8c-1d17-4dc1-8a26-16a47e0ce458" />
![1762344689264](https://github.com/user-attachments/assets/b306fa86-d1f4-45bb-8aa5-8b41680ef8be)
![1762344689243](https://github.com/user-attachments/assets/da8682aa-9ae3-496f-a90c-a5ad9eed568b)
![1762344689253](https://github.com/user-attachments/assets/e380289f-ebbb-4bf8-bb6a-6cd64d101630)
Made by:Nantachit Songsang(นันทชิต ส่งแสง)
