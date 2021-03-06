//
// Created by imanuel on 10.10.21.
//

#ifndef WXOCTO_OCTOAPIEVENTIDS_H
#define WXOCTO_OCTOAPIEVENTIDS_H

enum OctoApiEventId {
    OctoFilesFetched,
    OctoFilesError,
    OctoFileDeleted,
    OctoFileDeleteError,
    OctoFileUploaded,
    OctoFileUploadError,

    OctoJobFetched,
    OctoJobError,
    OctoPrintStarted,
    OctoPrintStartError,
    OctoPrintCancelled,
    OctoPrintCancelError,
    OctoPrintPaused,
    OctoPrintPauseError,
    OctoPrintResumed,
    OctoPrintResumeError,

    OctoPrintSpoolManagerSpoolsFetched,
    OctoPrintSpoolManagerSpoolsFetchError,
    OctoPrintSpoolManagerSpoolSaved,
    OctoPrintSpoolManagerSpoolSaveError,
    OctoPrintSpoolManagerSpoolDeleted,
    OctoPrintSpoolManagerSpoolDeleteError,
    OctoPrintSpoolManagerSpoolSelected,
    OctoPrintSpoolManagerSpoolSelectError
};

#endif //WXOCTO_OCTOAPIEVENTIDS_H
