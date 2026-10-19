#define LDR_PIN 32

void setup()                               
    {                                                   
        Serial.begin(115200);                           
        pinMode(LDR_PIN, INPUT);                       
    }            
                                           
    void loop()                                         
    {                                                   
        int lecturaADC = analogRead(LDR_PIN);          
        int porcentajeLuz = (lecturaADC * 100) / 4095; 

        Serial.print("ADC: ");                          
        Serial.print(lecturaADC);                       
        Serial.print(" | Luz: ");                       
        Serial.print(porcentajeLuz);                    
        Serial.println("%");         

        delay(1000);                                    
    } 