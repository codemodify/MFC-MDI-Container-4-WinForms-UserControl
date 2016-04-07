
#include "stdafx.h"
#include "ckgafx.h"
#include "MfcDocumentViewMdiWindowWithWinformsContent.h"

IMPLEMENT_DYNCREATE( IMfcMdiView, CView )

BEGIN_MESSAGE_MAP( IMfcMdiView, CView ) 
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP()
