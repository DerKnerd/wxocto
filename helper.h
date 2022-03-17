//
// Created by imanuel on 08.10.21.
//

#ifndef WXOCTO_HELPER_H
#define WXOCTO_HELPER_H

#ifdef WXC_FROM_DIP
#undef WXC_FROM_DIP
#endif
#if wxVERSION_NUMBER < 310
#define WXC_FROM_DIP(x) wxWindow::FromDIP(x, NULL)
#else
#define WXC_FROM_DIP(x) x
#endif

#endif //WXOCTO_HELPER_H
