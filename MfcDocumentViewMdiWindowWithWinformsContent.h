#pragma once

#include "ckgafx.h"
#include "LSTDOCVW.H"

// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----
//  MFC class that allows to serve as a MDI container for a WinForms control
// ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ---- ----

class _OKWAXEX_EXPORT MfcMdiFrame : public CMDIChildWndEx
{
    public:
        MfcMdiFrame() : CMDIChildWndEx()
        {}

        virtual ~MfcMdiFrame()
        {}
};

class _OKWAXEX_EXPORT IMfcMdiDocument : public CDocument
{
    public:
        IMfcMdiDocument() : CDocument()
        {}

        virtual ~IMfcMdiDocument()
        {}

        virtual BOOL OnNewDocument()
        {
            return TRUE;
        }
                                                                          
        virtual BOOL OnOpenDocument( LPCTSTR lpszPathName )
        {
           return TRUE;
        }

    	virtual void SetPathName( LPCTSTR lpszPathName, BOOL bAddToMRU = TRUE )
        {}

        gcroot<System::Windows::Forms::UserControl^> GetWinFormsControl()
        {
            System::Windows::Forms::UserControl^
                userControl = GetRootWinFormsControl();
            
            if( userControl->Controls->Count == 0 )
                return nullptr;

            return (System::Windows::Forms::UserControl^) userControl->Controls[ 0 ];
        }
        CWinFormsControl<System::Windows::Forms::UserControl>& GetCppWinFormsControl()
        {
            return _userControl;
        }

        void SetWinFormsControl( gcroot<System::Windows::Forms::UserControl^> newUserControl )
        {
            System::Windows::Forms::UserControl^ 
                userControl = GetRootWinFormsControl();
                userControl->Controls->Clear();
    
            newUserControl->Parent = userControl;
            newUserControl->Dock = System::Windows::Forms::DockStyle::Fill;

            CString title = newUserControl->Text;
            SetTitle( title );
        }

        virtual void OnSetWinFormsControl() {}

    private:
        CWinFormsControl<System::Windows::Forms::UserControl> _userControl;
        gcroot<System::Windows::Forms::UserControl^> GetRootWinFormsControl()
        {
            return static_cast<System::Windows::Forms::UserControl^>( _userControl.GetControl() );
        }
};

class _OKWAXEX_EXPORT IMfcMdiView : public CView
{
    DECLARE_DYNCREATE( IMfcMdiView )

    public:
        IMfcMdiView() : CView()
        {}

        virtual ~IMfcMdiView()
        {}

        virtual void OnDraw( CDC* pDC ) {}

        afx_msg int OnCreate( LPCREATESTRUCT lpCreateStruct )
        {
            if( CView::OnCreate(lpCreateStruct) == -1 )
                return -1;
 
            RECT rt; 
            this->GetClientRect( &rt ); 

            GetMfcMdiDocument()->GetCppWinFormsControl().CreateManagedControl( WS_VISIBLE, rt, this, 1 ); 

            GetMfcMdiDocument()->OnSetWinFormsControl();

            return 0; 
        }

        afx_msg void OnSize( UINT nType, int cx, int cy )
        {
            CView::OnSize( nType, cx, cy ); 
 
            RECT rt;
            this->GetClientRect( &rt );

            GetMfcMdiDocument()->GetCppWinFormsControl().MoveWindow( rt.left, rt.top, rt.right, rt.bottom, TRUE );
        }

        IMfcMdiDocument* GetMfcMdiDocument()
        {
            IMfcMdiDocument* document = (IMfcMdiDocument*) GetDocument();

            return document;
        }

    DECLARE_MESSAGE_MAP()
};

typedef CExMultiDocTemplate ExtendedMultiDocTemplate;
static const char* c_DocPath = "";
template<class T1, class T2> 
class _OKWAXEX_EXPORT MfcDocumentViewMdiWindowWithWinformsContentEx
{
    private:
        int _menuAndToolbarId;

    public:
        MfcDocumentViewMdiWindowWithWinformsContentEx( int menuAndToolbarId ) :
            _menuAndToolbarId( menuAndToolbarId )
        {}

        void OpenNew()
        {
            ExtendedMultiDocTemplate* 
                multiDocTemplate = AddDocTemplateIfNoneExists();
                multiDocTemplate->OpenDocumentFile( c_DocPath );
        }

        void ShowExisting( bool openNewIfNoneExists=true )
        {
            ExtendedMultiDocTemplate* multiDocTemplate = FindDocTemaplate();

            if( multiDocTemplate == NULL )
            {
                if( !openNewIfNoneExists )
                {
                    return;
                }
                else
                {
                    multiDocTemplate = AddDocTemplateIfNoneExists();
                }
            }

            CDocument* openDocument = NULL;
            multiDocTemplate->MatchDocType( c_DocPath, openDocument );
            
            if( openDocument == NULL )
            {
                multiDocTemplate->OpenDocumentFile( c_DocPath );
            }
            else
            {
                POSITION pos = openDocument->GetFirstViewPosition();
                if( pos != NULL )
                {
                    CView* view = openDocument->GetNextView( pos );
            
                    CFrameWnd* frameWindow = view->GetParentFrame();
                    if( frameWindow != NULL )
                    {
                        frameWindow->ActivateFrame();
                    } 
                    else
                    {
                        ASSERT( FALSE );
                    }
                }
            }
        }

        void CloseAll()
        {
            ExtendedMultiDocTemplate* multiDocTemplate = FindDocTemaplate();

            if( multiDocTemplate != NULL )
            {
                multiDocTemplate->CloseAllDocuments( TRUE );
            }
        }

        T1* GetDocument()
        {
            ExtendedMultiDocTemplate* multiDocTemplate = AddDocTemplateIfNoneExists();

            return static_cast<T1*>( multiDocTemplate->GetDocClass() );
        }

        T2* GetView()
        {
            ExtendedMultiDocTemplate* multiDocTemplate = AddDocTemplateIfNoneExists();

            return static_cast<T2*>( multiDocTemplate->GetViewClass() );
        }

    private:
        ExtendedMultiDocTemplate* FindDocTemaplate()
        {
            ExtendedMultiDocTemplate* multiDocTemplate = NULL;

            POSITION pos = AfxGetApp()->GetFirstDocTemplatePosition();
            while( pos != NULL )
            {
                CDocTemplate* docTemplate = AfxGetApp()->GetNextDocTemplate( pos );

                if( docTemplate->IsKindOf( RUNTIME_CLASS( ExtendedMultiDocTemplate ) ) )
                {
                    ExtendedMultiDocTemplate* docTemplateAsMultiDocTemplate = static_cast<ExtendedMultiDocTemplate*>( docTemplate );

                    if( (int) docTemplateAsMultiDocTemplate->GetnIDResource() == _menuAndToolbarId )
                    {
                        multiDocTemplate = docTemplateAsMultiDocTemplate;
                        break;
                    }
                }
            }

            return multiDocTemplate;
        }

        ExtendedMultiDocTemplate* AddDocTemplateIfNoneExists()
        {
            ExtendedMultiDocTemplate* multiDocTemplate = FindDocTemaplate();
            
            if( multiDocTemplate == NULL )
            {
                multiDocTemplate = new ExtendedMultiDocTemplate
                (
                    _menuAndToolbarId,
                    RUNTIME_CLASS( T1 ),
                    RUNTIME_CLASS( MfcMdiFrame ),
                    RUNTIME_CLASS( T2 )
                );

                AfxGetApp()->AddDocTemplate( multiDocTemplate );
            }

            return multiDocTemplate;
        }
};

template<class T1> 
class _OKWAXEX_EXPORT MfcDocumentViewMdiWindowWithWinformsContent : public MfcDocumentViewMdiWindowWithWinformsContentEx<T1,IMfcMdiView>
{
    public:
        MfcDocumentViewMdiWindowWithWinformsContent( int menuAndToolbarId ) :
            MfcDocumentViewMdiWindowWithWinformsContentEx( menuAndToolbarId )
        {}
};
