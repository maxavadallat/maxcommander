
.pragma library

// File List Popup Items
var fileListPopupItems          = [];

//==============================================================================
// Clear File List Popup Items
//==============================================================================
function clearFileListPopupItems()
{
    // Clear Items
    fileListPopupItems.splice(0, fileListPopupItems.length);
}

//==============================================================================
// Append File List Popup Item
//==============================================================================
function appendFileListPopupItem(item)
{
    // Push Item
    fileListPopupItems.push(item);
}

//==============================================================================
// Remove File List Popup Item
//==============================================================================
function removeFileListPopupItem(index)
{
    // Remove Item
    fileListPopupItems.splice(intex, 1);
}

//==============================================================================
// Check If File Is Image
//==============================================================================
function isImage(filePath, controller)
{
    // Get Last Index Of "."
    var lastIndexOfDot = filePath.lastIndexOf(".");
    // Check Last Index Of "."
    if (lastIndexOfDot >= 0) {
        // Extract Suffix
        var suffix = filePath.substr(lastIndexOfDot + 1, filePath.length - lastIndexOfDot - 1).toLowerCase();
        // Get Supported Image Formats
        var supportedFormats = controller.getSupportedImageFormats();
        // Check Suffix
        if (supportedFormats.indexOf(suffix) >= 0) {
            return true;
        }
    }

    return false;
}

//==============================================================================
// Format Seconds
//==============================================================================
function formatSecs(totalSecs)
{
    // Adjust Total Secs
    totalSecs = Math.round(totalSecs);
    // Get Hours
    var hours   = Math.floor(totalSecs / 3600);
    // Get Minutes
    var minutes = Math.floor((totalSecs - (hours * 3600)) / 60);
    // Get Seconds
    var seconds = totalSecs - (hours * 3600) - (minutes * 60);

    //console.log("formatSecs - totalSecs: " + totalSecs);

    // Round Secs
    seconds = Math.round(seconds);

    // Check Hours
    if (hours > 0 && hours < 10) {
        // Adjust Hours
        hours = "0" + hours.toString();
    }

    // Check Minutes
    if (minutes < 10) {
        // Adjust Minutes
        minutes = "0" + minutes.toString();
    }

    // Check Secs
    if (seconds < 10) {
        // Adjust Seconds
        seconds = "0" + seconds.toString();
    }

    // Check Hours
    if (hours > 0) {
        return hours + ":" + minutes + ":" + seconds;
    }

    return minutes + ":" + seconds;
}

//==============================================================================
// Color With Alpha
//==============================================================================
function colorWithAlpha(color, alpha)
{
    return color.slice(0, 1) + alpha + color.slice(3, color.length);
}


