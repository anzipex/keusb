#include <iostream>
#include <cstdlib>
#include <windows.h>

#define CONNECT_FILE 0
#define CONNECT_SIG 1
#define CONNECT_ANY 2
#define REQUEST_SIZE 200
#define READ_TIMEOUT 100
#define REPLY_PARTS 10

namespace {
static const char *airbag = "";
static const char *deviceName = "";
static char replyBuf[REQUEST_SIZE];
static char *replyPart[REPLY_PARTS];
static char deviceNameCom[] = "\\\\.\\COM?";
} // namespace

OVERLAPPED overlapped;

// Callback function for asynchronous write
void CALLBACK FileWriteCompletion(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped) {
    // Handling completion of write operation
    if (dwErrorCode == 0) {
        // Write operation completed successfully
        // std::cout << "FileWriteCompletion: " << dwNumberOfBytesTransfered << " bytes written" << std::endl;
    } else {
        // Handling write error
        std::cout << "FileWriteCompletion: ERROR_CODE " << dwErrorCode << std::endl;
    }
}

void PrintUsage() {
    std::cout << "Usage: keusb [option] [arguments]" << std::endl;
    std::cout << "Options:" << std::endl;
    std::cout << "  rele <relay_number> <on/off>: Turn on/off a relay. Arguments: <relay_number> (1-4), <on/off> (0 or 1)." << std::endl;
    std::cout << "  write <line_number> <value>: Write a value to a line. Arguments: <line_number> (1-18), <value> (0 or 1)." << std::endl;
    std::cout << "  write_array <array_of_values>: Write an array of values. Argument: <array_of_values> (e.g., \"000101\")." << std::endl;
    std::cout << "  hard_reset: Perform a factory reset." << std::endl;
}

class KeUsb {
public:
    /**
     *
     * @brief Initialization actions:
     * Opening the module;
     */
	KeUsb();

    /**
     * @brief Enable or disable the relay.
     *
     * @param rNum Relay number to turn on or off.
     * @param onOff Flag for turning on (1) or off (0).
     *
     * @return 0 on successful sending, 1 on error.
     */
    int keusbTurnOnOff(int rNum, int onOff);

    /**
     * @brief Sending a write value command.
     *
     * @param lineNumber Line number to which the value is written.
     * @param value Value to write.
     *
     * @return 0 on successful sending, 1 on error.
     */
    int keusbWrite(int lineNumber, int value);


    /**
     * @brief Sending an array values command.
     *
     * @param arrayOfValues Array of values to send.
     *
     * @return 0 on successful sending, 1 on error.
     */
    int keusbWriteArray(const char *arrayOfValues);

    /**
     * @brief Sending a command to reset all module states to default value.
     *
     * @return 0 on successful sending, 1 on error.
     */
    int keusbHardReset();

private:
	/**
	 * @brief Establish connection with KeUSB module.
	 *
	 * @param type Connection type (CONNECT_FILE, CONNECT_SIG, CONNECT_ANY).
	 * @param path Path to the device or signature for connection.
	 *
	 * @return 1 if connection is successful, 0 otherwise.
	 */
	int keusbConnect(int type, char *path);

    /**
     * @brief Converts a string from UTF-8 to LPCWSTR.
     *
     * @param narrowString Pointer to the string in narrow format (char*).
     *
     * @return Pointer to the string in LPCWSTR format.
     */
    LPCWSTR convertToLPCWSTR(const char *narrowString);

    /**
     * @brief Request handler.
     *
     * @note "..." - represents variadic arguments.
     *
     * @param command Accepts the command as a string
     * and some other arguments (variadic arguments) using "...".
     *
     * @return The function returns the value of tries + 1, which is the number of remaining attempts
     * (if all attempts were successful, 3 is returned, if one attempt was successful,
     * 2 is returned, and so on).
     */
    int keusbRequest(const char *command, ...);

    /**
     * @brief Writes a command to the device.
     *
     * @param buf Pointer to the buffer of data to be written to the device.
     * @param count Number of bytes of data to write.
     *
     * @return Number of bytes successfully written.
     */
    int deviceWrite(const void *buf, size_t count);

    /**
     * @brief Reads data from the device.
     *
     * @param buf Pointer to the buffer where the read data will be stored.
     * @param count Number of bytes of data to read.
     *
     * @return Number of bytes successfully read.
     */
    int deviceRead(void *buf, size_t count);

    /**
     * @brief Generates a unique device name. COM(1-256).
     *
     * @return Pointer to a string with the unique device name.
     */
    char *deviceGenName();

    /**
     * @brief Opens the device for communication.
     * Sets communication parameters such as baud rate, data bits, parity, and stop bits.
     * Sets timeouts for read and write operations. Returns 1 on successful opening, 0 otherwise.
     *
     * @param name The name of the device to open.
     *
     * @return 1 on successful opening, 0 otherwise.
     */
    int deviceOpen(const char *name);

    /**
     * @brief Closes the device. Closes the handle of the opened object.
     */
    void deviceClose();

    /**
     * @brief Obtains the device signature using the command "$KE,SER".
     *
     * @return Pointer to a string with the device signature.
     */
    char *keusbGetSignature();

    /**
     * @brief Converts a string to lowercase.
     *
     * @param str Pointer to the string to convert.
     *
     * @return Pointer to the original string with characters in lowercase.
     */
    char *strToLower(char *str);

    /**
     * @brief Handles a fatal error by printing a message to the standard error stream.
     *
     * @param str String with the error message.
     *
     * @return Always returns 0.
     */
    int die(const char *str);

	std::string comName_; ///< Name of the COM port device.
	HANDLE deviceFd_; ///< Device descriptor.
	int replySize_; ///< Size of the response.
};

KeUsb::KeUsb() :
comName_(),
deviceFd_(0),
replySize_(0) {
	// Opening the module
	keusbConnect(CONNECT_ANY, 0);
	keusbConnect(CONNECT_FILE, (char *)comName_.c_str());
}

LPCWSTR KeUsb::convertToLPCWSTR(const char *narrowString) {
    int len = MultiByteToWideChar(CP_UTF8, 0, narrowString, -1, nullptr, 0);
    wchar_t *wideString = new wchar_t[len];
    MultiByteToWideChar(CP_UTF8, 0, narrowString, -1, wideString, len);
    return wideString;
}

int KeUsb::keusbTurnOnOff(int rNum, int onOff) {
    return keusbRequest("$KE,REL,%d,%d", rNum, onOff)
        || die("keusbTurnOnOff: request failed");
}

int KeUsb::keusbWrite(int lineNumber, int value) {
    return keusbRequest("$KE,WR,%d,%d", lineNumber, value)
        || die("keusbWrite: request failed");
}

int KeUsb::keusbWriteArray(const char *arrayOfValues) {
    const char *commandFormat = "$KE,WRA,%s";
    char command[REQUEST_SIZE + 3];
    snprintf(command, REQUEST_SIZE, commandFormat, arrayOfValues);
    return keusbRequest(command)
        || die("keusbWriteArray: request failed");
}

int KeUsb::keusbHardReset() {
    return keusbRequest("$KE,RST")
        || die("keusbHardReset: request failed");
}

int KeUsb::keusbRequest(const char *command, ...) {
    va_list ap; // Variable of type va_list for working with variadic arguments.
    int length; // Stores the length of the request.
    int tries = 3; // Number of attempts.
    char request[REQUEST_SIZE + 3]; // Array for forming the request.

    va_start(ap, command);
    vsnprintf(request, REQUEST_SIZE, command, ap); // Forming the request string and storing it in the request array.
    va_end(ap); // Closing the work with variadic arguments.

    length = static_cast<int>(strlen(request)); // Calculating the length of the formed request string.
    request[length] = '\r'; // Adding a carriage return character.
    request[length + 1] = '\n'; // Adding a newline character.
    request[length + 2] = '\0'; // Adding a terminating null character.

    // Perform operations (deviceWrite) and (deviceRead) according to the number of attempts (tries).
    while (tries--) {
        int parts = 0;
        char *p, *q;

        replyBuf[0] = '\0';
        deviceWrite(request, length + 2); // Performing write operations of the command to the device (deviceWrite).
        deviceRead(replyBuf, REQUEST_SIZE); // Performing read operations of the response from the device (deviceRead).

        /**
        * Here the response from the device is checked.
        * Whether the response starts with "#ERR\r\n" or the first character of the response is not "#".
        * If one of the conditions is met, the loop continues again.
        */
        if (!strncmp(replyBuf, "#ERR\r\n", 6) || replyBuf[0] != '#') {
            continue;
        }

        /**
        * Inside the loop, the response is processed.
        * Iterating through the response (replyBuf), starting from the second character (replyBuf + 1).
        * If a comma (,) is encountered, the current part of the response is written to the replyPart array,
        * and the pointer q is set to the next part.
        */
        for (p = q = replyBuf + 1; *p != '\r'; p++) {
            if (*p == ',') {
                replyPart[parts++] = q;
                *p = '\0';
                q = p + 1;
            }
        }

        /**
        * After processing the response, a null character is set at the end of the response.
        * The response size (replySize_) is set, and the last part of the response is added to the replyPart array.
        * Then the loop exits.
        */
        *p = '\0';
        replySize_ = parts + 1;
        replyPart[parts] = q;
        break;
    }

    /**
    * The function returns the value of tries + 1, which is the number of remaining attempts (if all attempts were successful,
    * then 3 is returned, if one attempt was successful, then 2 is returned, and so on).
    */
    return tries + 1;
}

int KeUsb::deviceWrite(const void *buf, size_t count) {
    DWORD bytesWritten;

    if (WriteFileEx(deviceFd_, buf, static_cast<DWORD>(count), &overlapped, FileWriteCompletion)) {
        return 1;
    } else {
        std::cout << "WriteFileEx: write operation code " << GetLastError() << std::endl;
        return 0;
    }
}

int KeUsb::deviceRead(void *buf, size_t count) {
    DWORD bytesRead;

    Sleep(READ_TIMEOUT);
    if (!ReadFile(deviceFd_, buf, static_cast<DWORD>(count), &bytesRead, NULL)) {
        DWORD error = GetLastError();
        std::cout << "ReadFile: ERROR_CODE " << error << std::endl;
    }

    return bytesRead;
}

char *KeUsb::deviceGenName() {
    static int tryNum = 1;

    if (tryNum > 256) {
        return 0;
    }

    if (tryNum <= 9) {
        deviceNameCom[7] = '0' + tryNum++; // Units
    } else if (tryNum <= 99) {
        deviceNameCom[7] = '0' + (tryNum / 10); // Tens
        deviceNameCom[8] = '0' + (tryNum % 10); // Units
        tryNum++;
    } else {
        deviceNameCom[7] = '0' + (tryNum / 100); // Hundreds
        deviceNameCom[8] = '0' + ((tryNum / 10) % 10); // Tens
        deviceNameCom[9] = '0' + (tryNum % 10); // Units
        tryNum++;
    }

    return deviceNameCom;
}

int KeUsb::deviceOpen(const char *name) {
    DCB dcb;
    COMMTIMEOUTS commTimeOuts;

    LPCWSTR wideString = convertToLPCWSTR(name);
    deviceFd_ =
        CreateFile(wideString, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

    if (deviceFd_ == INVALID_HANDLE_VALUE) {
        return 0;
    }

    GetCommState(deviceFd_, &dcb);

    dcb.BaudRate = CBR_9600;
    dcb.ByteSize = 8;
    dcb.Parity = NOPARITY;
    dcb.StopBits = ONESTOPBIT;

    commTimeOuts.ReadIntervalTimeout = MAXDWORD;
    commTimeOuts.ReadTotalTimeoutMultiplier = 0;
    commTimeOuts.ReadTotalTimeoutConstant = 0;
    commTimeOuts.WriteTotalTimeoutMultiplier = 0;
    commTimeOuts.WriteTotalTimeoutConstant = 1000;

    SetCommTimeouts(deviceFd_, &commTimeOuts);
    SetCommState(deviceFd_, &dcb);

    return 1;
}

void KeUsb::deviceClose() {
    if (deviceFd_ != 0) {
        CloseHandle(deviceFd_);
        deviceFd_ = 0;
    }
}

char *KeUsb::keusbGetSignature() {
    if (!keusbRequest("$KE,SER") || replySize_ < 2) {
        int length = static_cast<int>(strlen(airbag));
        char *mutableAirbag = new char[length + 1];
        strcpy_s(mutableAirbag, length + 1, airbag);
        return mutableAirbag;
    }

    return strToLower(replyPart[1]);
}

char *KeUsb::strToLower(char *str) {
    char *p = str;

    while (*p && *p != ' ') {
        *p = tolower(*p);
        p++;
    }

    *p = '\0';
    return str;
}

int KeUsb::die(const char *str) {
    std::cerr << "error: " << str << std::endl;
    return 0;
}

int KeUsb::keusbConnect(int type, char *path) {
    char *name;
    deviceName = path ? path : airbag;

    if (type == CONNECT_FILE) {
        return (deviceOpen(path) && keusbRequest("$KE")) || die("keusbConnect: can't open device");
    }

    while ((name = deviceGenName())) {
        if (!deviceOpen(name)) {
            continue;
        }

        if (!keusbRequest("$KE")) {
            deviceClose();
            continue;
        }

        if (type == CONNECT_SIG && strcmp(keusbGetSignature(), strToLower(path))) {
            deviceClose();
            continue;
        }

        deviceName = name;
        if (type == CONNECT_ANY) {
            const char *digit_start = name;
            while (*digit_start && !isdigit(*digit_start)) {
                ++digit_start;
            }

            if (*digit_start) {
                std::string result;
                while (*digit_start && isdigit(*digit_start)) {
                    result += *digit_start;
                    ++digit_start;
                }

                comName_ = "\\\\.\\COM" + result;
            }
        }
        deviceClose();
        return 1;
    }
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        PrintUsage();
        return 1;
    }

    KeUsb keusb;

    std::string command = argv[1];
    if (command == "rele") {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " rele <rNum> <onOff>" << std::endl;
            return 1;
        }
        int rNum = std::atoi(argv[2]);
        int onOff = std::atoi(argv[3]);
        if (rNum < 1 || rNum > 4 || (onOff != 0 && onOff != 1)) {
            std::cerr << "Invalid arguments for rele command" << std::endl;
            return 1;
        }
        return keusb.keusbTurnOnOff(rNum, onOff);
    } else if (command == "write") {
        if (argc != 4) {
            std::cerr << "Usage: " << argv[0] << " write <lineNumber> <value>" << std::endl;
            return 1;
        }
        int lineNumber = std::atoi(argv[2]);
        int value = std::atoi(argv[3]);
        return keusb.keusbWrite(lineNumber, value);
    } else if (command == "write_array") {
        if (argc != 3) {
            std::cerr << "Usage: " << argv[0] << " write_array <arrayOfValues>" << std::endl;
            return 1;
        }
        return keusb.keusbWriteArray(argv[2]);
    } else if (command == "hard_reset") {
        if (argc != 2) {
            std::cerr << "Usage: " << argv[0] << " hard_reset" << std::endl;
            return 1;
        }
        return keusb.keusbHardReset();
    } else {
        std::cerr << "Unknown command: " << command << std::endl;
        return 1;
    }

	return 0;
};
