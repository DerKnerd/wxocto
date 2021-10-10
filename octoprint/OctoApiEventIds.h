//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_OCTOAPIEVENTIDS_H
#define WXOCTO_OCTOAPIEVENTIDS_H

enum OctoApiEventId {
    OctoFilesFetched,
    OctoFilesError,
    OctoJobFetched,
    OctoJobError,
    OctoPrintStarted,
    OctoPrintStartError,
    OctoPrintCancelled,
    OctoPrintCancelError,
    OctoPrintPaused,
    OctoPrintPauseError,
    OctoPrintResumed,
    OctoPrintResumeError
};

#endif //WXOCTO_OCTOAPIEVENTIDS_H
