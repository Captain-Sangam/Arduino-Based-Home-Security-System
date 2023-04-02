/*
||
|| @file Keypad.h
|| @version 2.0
|| @author Mark Stanley, Alexander Brevig
|| @contact mstanley@technologist.com, alexanderbrevig@gmail.com
||
|| @description
|| | This library provides a simple interface for using matrix
|| | keypads. It supports the use of multiple keypads with the
|| | same or different sets of keys.  It also supports user
|| | selectable pins and definable keymaps.
|| #
||
|| @license
|| | This library is free software; you can redistribute it and/or
|| | modify it under the terms of the GNU Lesser General Public
|| | License as published by the Free Software Foundation; version
|| | 2.1 of the License.
|| |
|| | This library is distributed in the hope that it will be useful,
|| | but WITHOUT ANY WARRANTY; without even the implied warranty of
|| | MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
|| | Lesser General Public License for more details.
|| |
|| | You should have received a copy of the GNU Lesser General Public
|| | License along with this library; if not, write to the Free Software
|| | Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
|| #
||
*/

#include <Keypad.h>

// <<constructor>> Allows custom keymap. pin configuration and keypad size
Keypad::Keypad(char *userKeymap, byte *row, byte *col, byte rows, byte cols) {
    rowPins = row;
    columnPins = col;

    size.rows = rows;
    size.columns = cols;
	curCol=0;
	curRow=0;

    begin(userKeymap);

    buttons = OPEN;
    debounceTime = 2;
    holdTime = 500;
	keypadEventListener = 0;

    transitionTo(IDLE);
    stateChanged = false;

    initializePins();
}

// New in 2.0 this function lets the end user test for any changes in state
// before deciding if any variables, etc. need to be updated in their code.
boolean Keypad::keyStateChanged() {
    return stateChanged;
}

// Let the user define a keymap - assume the same row/column count as defined in constructor
void Keypad::begin( char *userKeymap) {
    keymap = userKeymap;
}

char Keypad::getKey() {
	// Return the new key value if a keypress was detected. By testing for
	// keyStateChanged() we don't return a keypress more than once.
    if( getKeyState()==PRESSED && keyStateChanged() )
        return currentKey;

	return NO_KEY;	// Otherwise just return the default key value:
}

// Private
// Scan the keypad and report whether or not a key (or any key) has been pressed.
// If a key has been pressed then store its location in curCol and curRow.
// 2011-12-23 - Removed from getKeyState() for readability and ease of maintenance.
boolean Keypad::scanKeys() {
	static unsigned int allKeys=0;
	static byte c=0, r=0;
	byte curKey=0;

	// TODO Test with LCD Module installed and actually multiplex the data pins.
	// Assume that some other device is sharing (muxing) the pins used by the
	// keypad. If that is the case then the pins will need to be re-intialized
	// each time before they are used.
	initializePins();

	// I rewrote this method to scan a single row and a single column. This makes
	// a notable improvement in key selection. Finally, by sending the key selection
	// row and column to the state machine vastly improves key debouncing. Debounce
	// times on my very poor keypad have been reduce from 200mS to 4mS.
	for( int i=0; i<(size.rows*size.columns); i++) {
		digitalWrite(columnPins[c], LOW);
		curKey = digitalRead(rowPins[r]);
		allKeys += curKey;

		// Remember which key was pressed for use by getKeyState().
		if(curKey==0) {
			curCol = c;
			curRow = r;
		}
		// All keys have been scanned. Set 'buttons' value for use by getKeyState().
		if( r==(size.rows-1) && c==(size.columns-1) )
		{
			if( allKeys==(size.rows*size.columns) )
					buttons = OPEN;
			else
					buttons = CLOSED;
		}

		// Move to next row.
		r++;

		// Last row?  If yes, start again at the first row.
		if(r==size.rows) {
			r=0;
			digitalWrite(columnPins[c], HIGH);
			c++;
			if(c==size.columns) {
					c=0;
					allKeys = 0;
			}
		}
	}
	return buttons;		// True or false: Was *any* key pressed?
}

// Private
// I rewrote the state machine and renamed it to this function. The old getKey()
// method failed to return the IDLE state and was too tightly integrated with the
// key scan to make simple changes without breaking it completely. But most
// importantly only one key can ever be evaluated at any point in time.
KeyState Keypad::getKeyState() {
	static unsigned long startTime;
    static unsigned long Timer;
    stateChanged = false;

	// Scan keypad once every 10 mS. This makes the loop() count go from about 4,000 loops per
	// second to about 40,000 loops per second. A 10 fold increase in program speed. Most humans
	// can't press a key any faster than 20 mS so the end user won't notice any lag at 10 mS.
	if ( (millis()-startTime)>10 )
		startTime = millis();
	else
		return state;

	// Find both the key state, and row & column values.
	scanKeys();


// ------- Begin Key State Machine ---------
	switch (state) {
	case IDLE:
		// Waiting for a debounced keypress.
		if( buttons==CLOSED && (millis()-Timer)>debounceTime ) {
			currentKey = keymap[curCol+(curRow*size.columns)];		// This *must* come before transitionTo!
			transitionTo(PRESSED);
			Timer = millis();
		}
		break;
	case PRESSED:
		// Waiting for a key hold...
		if ( (millis()-Timer)>holdTime ) {
			transitionTo(HOLD);      // Move to next state.
			Timer = millis();    // Reset debounce timer.
		}
		// Or for the key to be release.
		else if ( buttons==OPEN && (millis()-Timer)>debounceTime ) {
			transitionTo(RELEASED);
			Timer = millis();
		}
		break;
	case HOLD:
		// Waiting for the key to be released.
		if ( (buttons==OPEN) && (millis()-Timer)>debounceTime ) {
			transitionTo(RELEASED);
			Timer = millis();
		}
		break;
	case RELEASED:
		transitionTo(IDLE);
		break;
	}
    return state;  			// Let the world know which state we're in.
}

KeyState Keypad::getState() {
	return state;
}

void Keypad::setDebounceTime(unsigned int debounce) {
    debounceTime = debounce;
}

void Keypad::setHoldTime(unsigned int hold) {
    holdTime = hold;
}

void Keypad::addEventListener(void (*listener)(char)){
	keypadEventListener = listener;
}

void Keypad::transitionTo(KeyState nextState) {
    state = nextState;
    stateChanged = true;
	if (keypadEventListener!=NULL){
		keypadEventListener(currentKey);
	}
}

void Keypad::initializePins() {
    //configure column pin modes and states
    for (byte C=0; C<size.columns; C++) {
        pinMode(columnPins[C],OUTPUT);
        digitalWrite(columnPins[C],LOW);
    }
    //configure row pin modes and states
    for (byte R=0; R<size.rows; R++) {
        pinMode(rowPins[R],INPUT);
		digitalWrite(rowPins[R],HIGH);	// Enable the internal 20K pullup resistors for each row pin.
    }
}

/*
|| @changelog
|| | 2011-12-23 - Mark Stanley : Rewrote state machine (Previously failed to set the IDLE state).
|| | 2011-12-23 - Mark Stanley : Significant speed improvements and removed state machine from getKey().
|| | 2011-11-29 - Tom Putzeys : Use internal pull-up, no  more column diodes needed, and consumes less power
|| | 2009-07-08 - Alexander Brevig : Library does not use 2d arrays
|| | 2009-06-15 - Alexander Brevig : Added transitionTo
|| | 2009-06-15 - Alexander Brevig : Added getState()
|| | 2009-06-13 - Mark Stanley : Fixed bug in getKey() that returns the wrong key if debounceTime is too short.
|| | 2009-06-13 - Mark Stanley : Minor bug fix:  Added 'currentKey = NO_KEY' to constructors.
|| | 2009-05-19 - Alexander Brevig : Added setHoldTime()
|| | 2009-05-15 - Alexander Brevig : Changed begin() amd getKey(), this Library should be operational.
|| | 2009-05-09 - Alexander Brevig : Changed getKey()
|| | 2009-04-28 - Alexander Brevig : Modified API, and made variables private
|| | 2007-XX-XX - Mark Stanley : Initial Release
|| #
*/
