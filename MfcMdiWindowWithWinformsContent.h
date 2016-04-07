#pragma once

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  MFC class that allows to serve as a MDI container for a WinForms control
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

template<class T> 
class MfcMdiWindowWithWinFormsContent : public CMDIChildWndEx
{
    public:

        MfcMdiWindowWithWinFormsContent( int mfcMenuId=-1 ) : 
            CMDIChildWndEx(),
            _mfcMenuId( mfcMenuId )
        {
            LPCTSTR registeredClass = AfxRegisterWndClass( CS_HREDRAW | CS_VREDRAW, NULL, (HBRUSH)(COLOR_WINDOW+1), NULL );
    
            CMDIFrameWnd* parent = static_cast<CMDIFrameWnd*>( AfxGetMainWnd() );
    
            CMDIChildWnd::Create( registeredClass, _T(""), WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, rectDefault, parent );
        }

        gcroot<T^> GetWinFormControl()
        {
            return static_cast<T^>( _userControl.GetControl() );
        }

        afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct )
        {
            if( CMDIChildWndEx::OnCreate(lpCreateStruct) == -1 )
                return -1;
 
            if( _mfcMenuId != -1 )
            {
                _menu.LoadMenu( _mfcMenuId );
                m_hMenuShared = _menu.GetSafeHmenu();
            }

            RECT rt; 
            this->GetClientRect( &rt ); 

            _userControl.CreateManagedControl( WS_VISIBLE, rt, this, 1 ); 

            CString title = _userControl->Text;
            SetWindowText( title );

            return 0; 
        }

        afx_msg void OnSize( UINT nType, int cx, int cy )
        {
            CMDIChildWndEx::OnSize( nType, cx, cy ); 
 
            RECT rt;
            this->GetClientRect( &rt );

            _userControl.MoveWindow( rt.left, rt.top, rt.right, rt.bottom, TRUE );
        }

    protected:
        CMenu _menu;
        int _mfcMenuId;
        CWinFormsControl<T> _userControl;

    DECLARE_MESSAGE_MAP()
};

BEGIN_TEMPLATE_MESSAGE_MAP( MfcMdiWindowWithWinFormsContent, T, CMDIChildWndEx ) 
    ON_WM_CREATE()
    ON_WM_SIZE()
END_MESSAGE_MAP() 
