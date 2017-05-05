#define ai_thermistor A0   

#define RESISTOR_NOMINAL 100000  // 100kOhm   
#define TEMP_NOMINAL 25  // temp. for nominal resistance (25 C)
#define MAX_SAMPLES 5 // Samples to get average temperature
#define BETA_COEFF 3950 // Beta coefficient
#define RESISTOR_SERIES 10000    
#define DELAY_TIME 1000

int samples[MAX_SAMPLES];
int i;
float average;
float steinhart;

void setup(void) {
	Serial.begin(9600);

	// Set reference voltage to 3.3 V for analog input (minimize noise)
	analogReference(EXTERNAL);
}

void loop(void) {
	i = 0;
	average = 0;
	steinhart = 0;

	// Get average of acutal temperature
	for (i = 0; i< MAX_SAMPLES; i++) {
		average += analogRead(ai_thermistor);
		delay(10);
	}
	average /= MAX_SAMPLES;

	Serial.print("Average analog reading: ");
	Serial.println(average);

	// Convert the value to resistance
	average = 1023 / average - 1;
	average = RESISTOR_SERIES / average;
	Serial.print("Thermistor resistance: ");
	Serial.println(average);

	steinhart = average / RESISTOR_NOMINAL;     // (R/Ro)
	steinhart = log(steinhart);                  // ln(R/Ro)
	steinhart /= BETA_COEFF;                   // 1/B * ln(R/Ro)
	steinhart += 1.0 / (TEMP_NOMINAL + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart;                 // Invert
	steinhart -= 273.15;                         // convert to C

	Serial.print("Temperature in C: ");
	Serial.print(steinhart);

	delay(DELAY_TIME);
}