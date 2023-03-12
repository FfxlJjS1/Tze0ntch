# Contents

[[Database controller]]

# Info
**Customizable database controller** is a controller, by which interpretator can working for creating and managing a specific database structure, that it wants.

## Principles of this controller
- Is concrete realization of [[Database controller#Abstract DB Controller|abstract db controller]];
- Provide to interpreter elementary methods for managing db file.

## Variables and Methods
**Variables:**

**Methods:**
- OpenFile(bool onlyForRead = true, bool makeEmptyFile = false);
- IsOpen();
- SeekToPoint(integer point, ios_base pointRelatePosition);
- TellPoint();
- SeekToG(integer point, ios_base pointRelatePosition);
- TellG();
- ReadUntilSymbol(char symbol);
- Read(char* object, integer sizeOf);
- Write(char* object, integer sizeOf);
- CloseFile().