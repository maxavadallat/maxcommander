#ifndef UTILITY
#define UTILITY

#include <QString>

// File Sorting Type
enum FileSortType {
    EFSName  = 0,
    EFSExt,
    EFEType,
    EFSSize,
    EFSDate,
    EFSOwner,
    EFSPerms
};


// Get Total Space In Directory
qint64 getTotalSpace(const QString& aDirPath);
// Get Free Space In Directory
qint64 getFreeSpace(const QString& aDirPath);

#endif // UTILITY

