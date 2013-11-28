#include <View.h>
#include <Font.h>
#include <MessageRunner.h>
#include <PopUpMenu.h>
#include <memory>
using namespace std;


class CView : public BView
{
public:
	CView(BRect rect);
	CView(BMessage* pMessage);

	virtual void AttachedToWindow();
	virtual void MessageReceived(BMessage* pMsg);
	virtual void Draw(BRect rect);
	virtual status_t Archive(BMessage* pData, bool bDeep= true) const;
	virtual void MouseDown(BPoint point);

	static CView* Instantiate(BMessage* pData);

private:
	enum { MSG_REFRESH_DISP= 'rfrd',  MSG_QUIT= 'quit', MSG_ABOUT= 'abut' };
	auto_ptr<BMessageRunner> m_pMessageRunner;
	int32 m_nCurrWorkspace;
	BFont m_Font;
	auto_ptr<BPopUpMenu> m_pPopupMenu;

	void Remove();
};
