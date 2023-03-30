// CS31 Project 3
// Kareem Dibs

#include <iostream>
#include <string>
#include <cctype>
#include <cassert>
#include <cmath>
using namespace std;

bool isValidUppercaseStateCode(string stateCode);
bool isValidPollString(string pollData);
int countSeats(string pollData, char party, int& seatCount);

int main() {
	assert(isValidPollString("CT5D,NY9R16D1I,VT,ne3r00D"));
	assert(!isValidPollString("ZT5D,NY9R16D1I,VT,ne3r00D"));
	int seats;
	seats = -999;    // so we can detect whether countSeats sets seats
	assert(countSeats("CT5D,NY9R16D1I,VT,ne3r00D", 'd', seats) == 0 && seats == 21);
	seats = -999;    // so we can detect whether countSeats changes seats
	assert(countSeats("CT5D,NY9R16D1I,VT,ne3r00D", '%', seats) == 2 && seats == -999);
	seats = 100;
	assert(countSeats("ca5d00i,ks3r44i", 'i', seats) == 0 && seats == 44);
	cerr << "All tests succeeded" << endl;
	
	/*for (;;) {
		string pollData;
		cout << "Enter a poll data string: " << endl;
		getline(cin, pollData);

		char partyLetter;
		cout << "Enter a party letter: ";
		cin >> partyLetter;
		cin.ignore(10000, '\n');

		cout << isValidPollString(pollData) << endl;

		/*char c;
		if (isValidPollString) {
			cout << "Enter a party letter: ";
			cin >> c;
		}*/

		/*int seatCount;
		cout << countSeats(pollData, partyLetter, seatCount) << endl;
		cout << seatCount << endl;

		if (pollData == "quit") {
			break;
		}
	}*/
}

// If argument is a two-uppercase-letter state code, returns true. Else, returns false.
bool isValidUppercaseStateCode(string stateCode)
{
	const string codes =
		"AL.AK.AZ.AR.CA.CO.CT.DE.FL.GA.HI.ID.IL.IN.IA.KS.KY."
		"LA.ME.MD.MA.MI.MN.MS.MO.MT.NE.NV.NH.NJ.NM.NY.NC.ND."
		"OH.OK.OR.PA.RI.SC.SD.TN.TX.UT.VT.VA.WA.WV.WI.WY";
	return (stateCode.size() == 2 &&
		stateCode.find('.') == string::npos &&  // no '.' in stateCode
		codes.find(stateCode) != string::npos);  // match found
}

// Returns true if pollData is a valid poll data string.
bool isValidPollString(string pollData) {
	string funcPollData;
	for (string::size_type i = 0; i != pollData.size(); i++) {
		if (pollData.at(i) == ' ') { // Test for any blanks in pollData; If any empty spaces exist, return false.
			return false;
		}
		funcPollData += toupper(pollData.at(i)); // Capitalize all letters in upperPollData so that it is easier to work with.
	}

	if (funcPollData.size() < 1) { // If the size of funcPollData is less than 1, it is still a valid poll data string.
		return true;
	}

	// Analyze each individual state forecast by separating state forecasts by the commas in the poll data string.
	string stateForecast = "";
	funcPollData += ","; // For this program's purposes, allows the last state forecast to be analyzed as well.

	for (string::size_type j = 0; j != funcPollData.size(); j++) {
		stateForecast += funcPollData[j]; // For j in funcPollData, add j to stateForecast.

		// Once upperPollData approaches a comma, the state forecast preceding it is ready to be analyzed.
		if (funcPollData.at(j) == ',') {
			// Isolate first two characters of stateForecast into validState to analyze state code.
			string validState = stateForecast.substr(0, 2);

			// Verify if correctState is a valid uppercase state code.
			if (!isValidUppercaseStateCode(validState)) {
				return false;
			}

			// Analyze party letters to ensure that there are no consecutive letters after the first 2 letters of the state code.
			if (stateForecast.size() > 3) {
				// Check if a digit follows the state code. If not, return false.
				if (!isdigit(stateForecast.at(2))) {
					return false;
				}

				// Except for the last two characters of a state forecast, check if the party code is only one letter.
				// Start at index 2 and analyze stateForecast until the two last chars. These will be tested later.
				int n = stateForecast.size() - 2;
				for (string::size_type k = 2; k != n; k++) {
					if (isalpha(stateForecast.at(k))) { // Identify party letter and check if there is a digit right before it and a digit right after it.
						if ((!isdigit(stateForecast.at(k + 1))) || (!isdigit(stateForecast.at(k - 1)))) {
							return false;
						}
					}
					else if (!isdigit(stateForecast.at(k))) { // Checks to see if there are any other characters in stateForecast than digits or letters.
						return false;
					}
				}

				// Analyze the last 2 characters in stateForecast to make sure there is a valid party code and digit.
				if (!isalpha(funcPollData.at(j - 1))) {
					return false;
				}
				if (!isdigit(funcPollData.at(j - 2))) {
					return false;
				}
			}
			// After completing the analysis of a stateForecast, reset the StateForecast to an empty string to allow for the next analysis.
			stateForecast = "";
		}
	}
	// If all tests have been passed, the poll data string has proper syntax. Thus, return true.
	return true;
}

// Calculates the number of seats a certain party is expected to win.
int countSeats(string pollData, char party, int& seatCount) {
	
	if (!isValidPollString(pollData)) {
		return 1; // Checks syntax of pollData. If not proper syntax, return 1.
	}
	if (!isalpha(party)) {
		return 2; // Checks syntax of party code. If not proper syntax, return 2.
	}

	int pollDataSum = 0;

	// For ease of use, capitalize the characters in poll data.
	string uppercasePollData;
	for (string::size_type k = 0; k != pollData.size(); k++)
		uppercasePollData += toupper(pollData.at(k));

	// For ease of use, capitalize the party code.
	string uppercaseParty = "";
	uppercaseParty += toupper(party);

	uppercasePollData += ","; // Allows last stateForecast to be analyzed equally.
	string stateForecast = "";

	int n = uppercasePollData.size();

	for (string::size_type k = 0; k != n; k++) {
		stateForecast += uppercasePollData.at(k);
		if (uppercasePollData.at(k) == ',') { // State forecast is ready to be analyzed when you approach a comma.
			if (stateForecast.size() < 4) { // Indicates no forecasts, so skip to the next.
				stateForecast = "";
				continue;
			}

			// Time to analyze only the state forecasts that actually contain forecasts.
			if (stateForecast.size() > 3) {
				int sumStateForecast = 0;
				for (string::size_type a = 2; a != stateForecast.size(); a++) { // Analysis of stateForecast starting at index 2 (skipping state code).
					if (stateForecast.at(a) == uppercaseParty.at(0)) {
						string numberStateForecast = ""; // Calculates the number of party votes
						for (string::size_type q = (a - 1); q > 1; q--) { // Copies digit characters in reverse
							if (isalpha(stateForecast.at(q)))
								break; // Breaks out of for loop if q is a letter.
							// Converts string party vote number into char in reverse.
							numberStateForecast += stateForecast.at(q); // Reverses the characters.

						}

						int conversion;
						//8- Convert char into int
						for (string::size_type z = 0; z != numberStateForecast.size(); z++) {
							conversion = numberStateForecast.at(z);
							switch (conversion)
							{
							case 48: //ASCII
							case 240: //EBCDIC
								conversion = 0;
								break;
							case 49:
							case 241:
								conversion = 1;
								break;
							case 50:
							case 242:
								conversion = 2;
								break;
							case 51:
							case 243:
								conversion = 3;
								break;
							case 52:
							case 244:
								conversion = 4;
								break;
							case 53:
							case 245:
								conversion = 5;
								break;
							case 54:
							case 246:
								conversion = 6;
								break;
							case 55:
							case 247:
								conversion = 7;
								break;
							case 56:
							case 248:
								conversion = 8;
								break;
							case 57:
							case 249:
								conversion = 9;
								break;
							}
							// Determine pollDataSum based on the above conversions.
							if (z == 0)
								sumStateForecast += conversion;
							else
							{
								int power = pow(10.0, z);
								sumStateForecast += (conversion * power);
							}
						}
					}
				}
				// This process is repeated for all state forecasts. At the end, a sum of votes is obtained and stored in pollDataSum.
				pollDataSum += sumStateForecast;
			}
			stateForecast = ""; // After completing the analysis of a stateForecast, reset the StateForecast to an empty string to allow for the next analysis.
		}
	}
	// Total number of votes for specific party is assigned to seatCount.
	seatCount = pollDataSum;
	return 0;
}