/* Button scanning code, for reading from Cherry MX switch
   Code derived from Button Pad Hookup Guide Example 2
   by Byron Jacquot @ SparkFun Electronics
     https://learn.sparkfun.com/tutorials/button-pad-hookup-guide#exercise-2-monochrome-plus-buttons
*/
//////////////////////
// Config Variables //
//////////////////////
#define NUM_LED_COLS (0) // Number of LED columns (+, anode)
#define NUM_LED_ROWS (0) // Number of LED rows (-, cathode)
#define NUM_BTN_COLS (1) // Number of switch columns (isolating diode anode)
#define NUM_BTN_ROWS (1) // Number of switch rows (isolating diode cathode)
int potPin = 5;    // select the input pin for the potentiometer
int potValue = 0;  // variable to store the value coming from the sensor



// Debounce built-in to the code. This sets the number of button
// high or low senses that trigger a press or release
#define MAX_DEBOUNCE (3)

////////////////////
// Hardware Setup //
////////////////////
static const uint8_t btnRowPins[NUM_BTN_ROWS] = {9}; // Pins connected to switch rows (2)
static const uint8_t btnColPins[NUM_BTN_COLS] = {9}; // Pins connected to switch columns (1)
static const uint8_t ledRowPins[NUM_LED_ROWS] = {}; // Pins connected to LED rows (-)
static const uint8_t ledColPins[NUM_LED_COLS] = {}; // Pins connected to LED cols (+)

//////////////////////
// Global Variables //
//////////////////////
static bool LED_buffer[NUM_LED_COLS][NUM_LED_ROWS]; // Keeps track of LED states
static int8_t debounce_count[NUM_BTN_COLS][NUM_BTN_ROWS]; // One debounce counter per switch

void setup()
{
  Serial.begin(9600);

  setupSwitchPins();

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW


  // Initialize the debounce counter array
  for (uint8_t i = 0; i < NUM_BTN_COLS; i++)
  {
    for (uint8_t j = 0; j < NUM_BTN_ROWS; j++)
    {
      debounce_count[i][j] = 0;
    }
  }
}

void loop() 
{
  scan();
}

static void scan()
{
  // Each run through the scan function operates on a single row
  // of the matrix, kept track of using the currentRow variable.
  static uint8_t currentRow = 0;
  uint8_t i, j; // for loop counters

  // Select current row, and write all components on that row LOW.
  // That'll set the LED anode's LOW, and write the switch "2" pins LOW.
  // If diodes were added, "2' should be connected to the diode cathode
  digitalWrite(btnRowPins[currentRow], LOW);

  // Scan through switches on this row:
  for ( j = 0; j < NUM_BTN_COLS; j++)
  {
    // Read the button. If it's pressed, it should be LOW.
    if (digitalRead(btnColPins[j]) == LOW)
    {
      if ( debounce_count[currentRow][j] < MAX_DEBOUNCE)
      { // Increment a debounce counter
        debounce_count[currentRow][j]++;
        if ( debounce_count[currentRow][j] == MAX_DEBOUNCE )
        { // If debounce counter hits MAX_DEBOUNCE (default: 3)
          // Trigger key press -- Do anything here...
          Serial.print("capture "); 
          potValue = analogRead(potPin);
          Serial.println(potValue);
          //Serial.print(" ");
          digitalWrite(LED_BUILTIN, HIGH); 
          //Serial.println((currentRow * NUM_BTN_COLS) + j);
        }
      }
    }
    else // Otherwise, button is released
    {
      if ( debounce_count[currentRow][j] > 0)
      { 
        debounce_count[currentRow][j]--; // Decrement debounce counter
        if ( debounce_count[currentRow][j] == 0 )
        { // If debounce counter hits 0
          // Trigger key release -- Do anything here...
          //Serial.print("Key released "); // Trigger key release
          digitalWrite(LED_BUILTIN, LOW);  
          //Serial.println((currentRow * NUM_BTN_COLS) + j);
        }
      }
    }
  }

  // Once done scanning, de-select the switch rows
  // by writing them HIGH.
  digitalWrite(btnRowPins[currentRow], HIGH);

}

static void setupSwitchPins()
{
  uint8_t i;

  // Button drive rows - written LOW when active, HIGH otherwise
  for (i = 0; i < NUM_BTN_ROWS; i++)
  {
    pinMode(btnRowPins[i], OUTPUT);

    // with nothing selected by default
    digitalWrite(btnRowPins[i], HIGH);
  }

  // Buttn select columns. Pulled high through resistor. Will be LOW when active
  for (i = 0; i < NUM_BTN_COLS; i++)
  {
    pinMode(btnColPins[i], INPUT_PULLUP);
  }
}
