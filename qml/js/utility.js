
// Check If File Is Image
function isImage(filePath)
{
    // Get Last Index Of "."
    var lastIndexOfDot = filePath.lastIndexOf(".");
    // Check Last Index Of "."
    if (lastIndexOfDot >= 0) {
        // Extract Suffix
        var suffix = filePath.substr(lastIndexOfDot + 1, filePath.length - lastIndexOfDot - 1).toLowerCase();
        // Get Supported Image Formats
        var supportedFormats = mainController.getSupportedImageFormats();
        // Check Suffix
        if (supportedFormats.indexOf(suffix) >= 0) {
            return true;
        }
    }

    return false;
}

