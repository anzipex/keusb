# Ke-USB24R

This project is a forked version of the Ke-USB24R module driver, with modifications aimed at enhancing its functionality.

## Compatibility:
This program is compatible with Windows OS only.

## Key Modifications:

1. **Automatic COM port detection:** The COM port number is now automatically determined from 1 to 256, instead of the previous range of 1 to 9.
2. **Asynchronous write using WriteFileEx:** Asynchronous writing is implemented using WriteFileEx, allowing non-blocking calls when simultaneous write commands are necessary. This resolves issues such as program freezes observed in Boxer24R.exe during concurrent write calls.
3. **Array writing support:** Array writing functionality has been added, allowing for more versatile data transmission.

This implementation serves as an example of usage and can be further customized to suit individual needs. The code provided in the main function demonstrates typical usage of command calls.

## How to Use:

```
keusb [option] [arguments]
Options:
  rele <relay_number> <on/off>: Turn on/off a relay. Arguments: <relay_number> (1-4), <on/off> (0 or 1).
  write <line_number> <value>: Write a value to a line. Arguments: <line_number> (1-18), <value> (0 or 1).
  write_array <array_of_values>: Write an array of values. Argument: <array_of_values> (e.g., "000101").
  hard_reset: Perform a factory reset.
```

This version provides an overview of the modifications made to the original Ke-USB24R driver, along with instructions on how to use the program.
