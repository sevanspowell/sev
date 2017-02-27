#include <sv/resource/ConfigResourceLoader.h>

extern "C" {
#include <sv/console/Commands.h>
}

namespace sv {
    /// Returns true if the last two tokens constitute a valid command separator
    static bool isCommandSeparator(char lastTwo[2]) {
        return (lastTwo[0] == ' ' && svCommandsIsSep(lastTwo[1]));
    }
    
std::string ConfigResourceLoader::getPattern() const { return "*.cfg"; }

bool ConfigResourceLoader::useRawFile() const { return false; }

bool ConfigResourceLoader::discardRawBufferAfterLoad() const { return true; }

size_t ConfigResourceLoader::getLoadedResourceSize(const void *rawBuffer,
                                                   size_t rawBufferSize) const {
    const char *charBuffer = (const char *)rawBuffer;
    size_t numChars        = rawBufferSize / sizeof(char);

    size_t loadedSize = 0;
    char prevChars[2] = {'\0', '\0'};
    for (int i = 0; i < numChars; ++i) {
        if (charBuffer[i] == '\n' && !isCommandSeparator(prevChars)) {
            // Current character is end of line and has not been properly
            // terminated already

            // Replace each newline with a space and a command separator and a space
            // i.e. hello\n -> hello ;
            loadedSize += 3;
        } else {
            loadedSize += 1;
        }
        
        prevChars[0] = prevChars[1];
        prevChars[1] = charBuffer[i];
    }
    
    // Has line just before end of input been terminated correctly?
    if (!isCommandSeparator(prevChars)) {
        // If not, make space to terminate it properly
        loadedSize += 3;
    }

    loadedSize *= sizeof(char);

    return loadedSize;
}

/// \copydoc ResourceLoader::loadResource
bool ConfigResourceLoader::loadResource(
    const void *rawBuffer, size_t rawSize,
    std::shared_ptr<ResourceHandle> &handle) {

    const char *bufferIn = (const char *)rawBuffer;
    char *bufferOut      = (char *)handle->getMutableResourceBuffer();

    size_t numCharsIn = rawSize / sizeof(char);

    char prevChars[2] = {'\0', '\0'};
    size_t j         = 0;
    for (size_t i = 0; i < numCharsIn; ++i) {
        if (bufferIn[i] == '\n' && !isCommandSeparator(prevChars)) {
            // Current character is end of line and has not been properly
            // terminated already

            // Replace each newline with a space and a command separator and a space
            // i.e. hello\n -> hello ;
            bufferOut[j] = ' ';
            ++j;
            bufferOut[j] = *(SV_SEP_SEQ);
            ++j;
            bufferOut[j] = ' ';
            ++j;
        } else {
            bufferOut[j] = bufferIn[i];
            ++j;
        }

        prevChars[0] = prevChars[1];
        prevChars[1] = bufferIn[i];
    }

    // Has line just before end of input been terminated correctly?
    if (!isCommandSeparator(prevChars)) {
        // If not, terminate it properly
        bufferOut[j] = ' ';
        ++j;
        bufferOut[j] = *(SV_SEP_SEQ);
        ++j;
        bufferOut[j] = ' ';
        ++j;
    }
    
    return true;
}
}
