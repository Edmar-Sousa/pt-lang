#include "buffer.h"


char getNextChar() {
    if (bufferPos < bufferSize)
        return buffer[bufferPos++];

    if (fgets(buffer, MAX_BUFFER_SIZE - 1, source)) {
        bufferPos = 0;
        bufferSize = strlen(buffer);
        return buffer[bufferPos++];
    }

    return TOK_EOF;
}


void backCaracter()
{
    bufferPos -= 1;
}
