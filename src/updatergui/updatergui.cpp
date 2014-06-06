#include "../updaterlib/updaterlib.h"
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <Commctrl.h>
namespace Ui {

	struct Window;
	typedef std::shared_ptr<Window> WindowPtr;
	struct Widget;
	typedef std::shared_ptr<Widget> WidgetPtr;

	struct Widget {

		virtual ~Widget() {
		}

		void show() {
			ShowWindow(h_, 1);
		}

		WindowPtr getParent() {
			return parent_.lock();
		}

		RECT getWindowRect() const {
			RECT rc;
			GetWindowRect(h_, &rc);
			return rc;
		}

		void centerOnScreen() {
			int screenWidth = GetSystemMetrics(SM_CXSCREEN);
			int screenHeight = GetSystemMetrics(SM_CYSCREEN);
			const RECT rc = getWindowRect();
			int currentWidth = rc.right - rc.left;
			int currentHeight = rc.bottom - rc.top;
			SetWindowPos(h_, 0, (screenWidth - currentWidth) / 2, (screenHeight - currentHeight) / 2, 0, 0, SWP_NOZORDER | SWP_NOSIZE);
		}
	protected:

		HWND getHandle() const {
			return h_;
		}

		static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
			Widget *wnd = nullptr;
			if (uMsg == WM_CREATE) {
				CREATESTRUCT *pCreate = reinterpret_cast<CREATESTRUCT*> (lParam);
				wnd = reinterpret_cast<Widget*> (pCreate->lpCreateParams);
				SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) wnd);
			} else {
				LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
				wnd = reinterpret_cast<Widget*> (ptr);
			}
			if (!wnd) return DefWindowProc(hwnd, uMsg, wParam, lParam);
			return wnd->onMsg(uMsg, wParam, lParam);
		}
	private:
		friend class Gui;
		HWND h_;
		std::weak_ptr<Window> parent_;
		virtual DWORD getStyle()const = 0;
		virtual const char *getClassName() const = 0;
		bool create(WindowPtr parent, HINSTANCE hInstance, const char *text, int szx, int szy, int px, int py);

		virtual LRESULT onDestroy(WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(h_, WM_DESTROY, wParam, lParam);
		}

		virtual LRESULT onPaint(WPARAM wParam, LPARAM lParam) {
			return DefWindowProc(h_, WM_PAINT, wParam, lParam);
		}

		virtual LRESULT onMsg(UINT uMsg, WPARAM wParam, LPARAM lParam) {
			switch (uMsg) {
					//case WM_CREATE:
					//InvalidateRect(h_, 0, true);
					//break;
				case WM_DESTROY:
					return onDestroy(wParam, lParam);
				case WM_PAINT:
					return onPaint(wParam, lParam);
				default: break;
			}

			return DefWindowProc(h_, uMsg, wParam, lParam);
		}
	};

	struct Window : public Widget {

		void close() {
			DestroyWindow(getHandle());
		}
	private:
		friend class Gui;
		constexpr static const char* CLASS_NAME = "Some window";

		const char *getClassName() const override {
			return CLASS_NAME;
		}

		DWORD getStyle() const override {
			return WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU; // | WS_VISIBLE;
		}

		LRESULT onDestroy(WPARAM /*wParam*/, LPARAM /*lParam*/) override {
			PostQuitMessage(0);
			return 0;
		}

		//		LRESULT onPaint(WPARAM /*wParam*/, LPARAM /*lParam*/) override {
		//			PAINTSTRUCT ps;
		//			HDC hdc = BeginPaint(getHandle(), &ps);
		//			FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));
		//			EndPaint(getHandle(), &ps);
		//			return 0;
		//		}

		static void registerClass(HINSTANCE hInstance) {
			WNDCLASS wc;
			wc.cbClsExtra = 0;
			wc.cbWndExtra = 0;
			wc.hbrBackground = (HBRUSH) (COLOR_BTNFACE + 1);
			wc.hCursor = LoadCursor(NULL, IDC_ARROW);
			wc.hIcon = LoadIcon(0, IDI_WINLOGO); //LoadIcon(NULL, IDI_APPLICATION);
			wc.hInstance = hInstance;
			wc.lpfnWndProc = Widget::WindowProc;
			wc.lpszClassName = CLASS_NAME;
			wc.lpszMenuName = 0;
			wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
			RegisterClass(&wc);
		}
	};

	struct ProgressBar : public Widget {

		void setRange(int low, int high) {
			SendMessage(getHandle(), PBM_SETRANGE, 0, MAKELPARAM(low, high));
		}

		void setProgress(int v) {
			SendMessage(getHandle(), PBM_SETPOS, v, 0);
		}
	private:

		const char *getClassName() const override {
			return PROGRESS_CLASS;
		}

		DWORD getStyle() const override {
			return WS_CHILD | WS_VISIBLE;
		}
	};
	typedef std::shared_ptr<ProgressBar> ProgressBarPtr;

	struct Label : public Widget {

		void setText(const char *text) {
			SetWindowText(getHandle(), text);
		}
	private:

		const char *getClassName() const override {
			return "STATIC";
		}

		DWORD getStyle() const override {
			return WS_CHILD | WS_VISIBLE | SS_LEFT;
		}
	};
	typedef std::shared_ptr<Label> LabelPtr;

	bool Widget::create(WindowPtr parent, HINSTANCE hInstance, const char *text, int szx, int szy, int px, int py) {
		parent_ = parent;
		h_ = CreateWindowEx(
				  0, // Optional window styles.
				  getClassName(), // Window class
				  text, // Window text
				  getStyle(), // Window style
				  px, py, // position 
				  szx, szy, // size 
				  parent ? parent->h_ : NULL, // Parent window    
				  NULL, // Menu
				  hInstance, // Instance handle
				  dynamic_cast<Widget*> (this) // Additional application data
				  );

		return (h_ != NULL);
	}

	struct Gui {

		Gui() {
			INITCOMMONCONTROLSEX st;
			st.dwSize = sizeof (st);
			st.dwICC = ICC_STANDARD_CLASSES;
			InitCommonControlsEx(&st);
		}

		bool update() {
			MSG msg;
			while (PeekMessage(&msg, 0, 0, 0, PM_NOREMOVE)) {
				if (!GetMessage(&msg, 0, 0, 0)) {
					return false;
				}
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}
			return true;
		}

		WindowPtr createWindow(const char *title, int szx, int szy, int px, int py) {
			if (!windowRegistered_) {
				Window::registerClass(hInstance_);
				windowRegistered_ = true;
			}
			WindowPtr ret = std::make_shared<Window>();
			if (ret->create(nullptr, hInstance_, title, szx, szy, px, py)) {
				return ret;
			}
			return
			{
			};
		}

		ProgressBarPtr createProgressBar(WindowPtr parent, int szx, int szy, int px, int py) {
			ProgressBarPtr ret = std::make_shared<ProgressBar>();
			szy = GetSystemMetrics(SM_CYVSCROLL);
			if (ret->create(parent, hInstance_, nullptr, szx, szy, px, py)) {
				return ret;
			}
			return
			{
			};
		}

		LabelPtr createLabel(WindowPtr parent, int szx, int szy, int px, int py) {
			LabelPtr ret = std::make_shared<Label>();
			if (ret->create(parent, hInstance_, nullptr, szx, szy, px, py)) {
				return ret;
			}
			return
			{
			};
		}
	private:
		bool windowRegistered_ = false;
		const HMODULE hInstance_ = GetModuleHandle(NULL);
	};
}

namespace Imp {

	struct UpdateWindow {

		void init(Ui::Gui &gui) {
			int width = 450;
			int height = 140;
			wnd_ = gui.createWindow("Updater", width, height, 0, 0);
			wnd_->centerOnScreen();
			wnd_->show();
			int i = 4;
			int w = width - 12;
			int lblh = 18;
			int h = 15;
			int dlbl = 20;
			int d = 40;
			label1_ = gui.createLabel(wnd_, w, lblh, 3, i);
			i += dlbl;
			progress1_ = gui.createProgressBar(wnd_, w, h, 3, i);
			i += d;
			label2_ = gui.createLabel(wnd_, w, lblh, 3, i);
			i += dlbl;
			progress2_ = gui.createProgressBar(wnd_, w, h, 3, i);
		}

		void close() {
			wnd_->close();
		}

		template<typename... Args> void setText1(Args&&... args) {
			std::string text = toStr(std::forward<Args>(args)...);
			label1_->setText(text.c_str());
		}

		template<typename... Args> void setText2(Args&&... args) {
			std::string text = toStr(std::forward<Args>(args)...);
			label2_->setText(text.c_str());
		}

		void setProgress1(int i) {
			progress1_->setProgress(i);
		}

		void setProgress2(int i) {
			progress2_->setProgress(i);
		}
	private:
		Ui::WindowPtr wnd_;
		Ui::LabelPtr label1_;
		Ui::ProgressBarPtr progress1_;
		Ui::LabelPtr label2_;
		Ui::ProgressBarPtr progress2_;
	};

	static std::string humanReadableSize(std::size_t sz) {

		struct Mul {
			std::size_t mult_;
			const char *suffix_;
		};
		constexpr std::size_t m = 1024;
		constexpr Mul muls[] = {
			//{m * m * m * m, "Tb"},
			{m * m * m, "Gb"},
			{m * m, "Mb"},
			{m, "Kb"}
		};
		for (const Mul &mul : muls) {
			if (sz > mul.mult_ * 0.8) {
				return toStr(std::fixed, std::setprecision(2), double(sz) / mul.mult_, " ", mul.suffix_);
			}
		}
		return toStr(sz, " b");
	}

	struct MyListener : public Updater::Listener {

		MyListener(UpdateWindow &wnd) : wnd_(wnd) {

		}

		void loadingCache() override {
			wnd_.setText1("Loading cache...");
		}

		void connecting(const std::string &/*address*/, const std::string&/*port*/) override {
			wnd_.setText1("Connecting to updating site...");
		}

		void couldntConnect() override {
			wnd_.setText1("Couldn't connect to updating site!");
		}

		void requestingFileData() override {
			wnd_.setText1("Requesting list of updates...");
		}

		void updaterUpdateRequired(std::size_t totalSize) override {
			downloadingUpdater_ = true;
			currentFileIndex_ = 0;
			totalFiles_ = 1;
			totalSize_ = totalSize;
			accumulatedSize_ = 0;
			updateTotals();
		}

		void updatesRequired(std::size_t numFiles, std::size_t totalSize) override {
			currentFileIndex_ = 0;
			totalFiles_ = numFiles;
			totalSize_ = totalSize;
			accumulatedSize_ = 0;
			updateTotals();
		}

		void noUpdatesRequired() override {
			wnd_.setText1("No updates found");
		}

		void requestingFile(const std::string &name, std::size_t sz) override {
			currentFileName_ = name;
			currentFileSize_ = 0;
			currentFileTotalSize_ = sz;
			updateFile();
		}

		void gotChunk(const std::size_t sz) override {
			accumulatedSize_ += sz;
			currentFileSize_ += sz;
			updateFile();
		}

		void fileFinished() override {
			wnd_.setText2("");
			wnd_.setProgress2(0);
			++currentFileIndex_;
		}

		void jobDone() override {
			wnd_.close();
		}
	private:
		UpdateWindow &wnd_;
		bool downloadingUpdater_ = false;
		std::string currentFileName_;
		std::size_t totalFiles_;
		std::size_t totalSize_;
		std::size_t currentFileTotalSize_;
		std::size_t currentFileSize_;
		std::size_t currentFileIndex_;
		std::size_t accumulatedSize_;

		void updateTotals() {
			if (downloadingUpdater_) {
				const int currentPct = currentFileSize_ / (currentFileTotalSize_ / double(100));
				wnd_.setProgress1(currentPct);
				wnd_.setText1("Downloading new updater");
			} else {
				const int currentTotalPct = accumulatedSize_ / (totalSize_ / double(100));
				wnd_.setProgress1(currentTotalPct);
				wnd_.setText1("Downloading file ", currentFileIndex_, " of ", totalFiles_, " (", humanReadableSize(accumulatedSize_), " of ", humanReadableSize(totalSize_), ")");
			}
		}

		void updateFile() {
			const int currentPct = currentFileSize_ / (currentFileTotalSize_ / double(100));
			wnd_.setProgress2(currentPct);
			wnd_.setText2("Retrieving ", currentFileName_, " (", humanReadableSize(currentFileSize_), " of ", humanReadableSize(currentFileTotalSize_), ")");				
			updateTotals();
		}
	};
}

int main() {
	try {
		Ui::Gui gui;
		Imp::UpdateWindow updateWindow;
		updateWindow.init(gui);
		Imp::MyListener listener(updateWindow);
		std::thread th([&listener]() {
			Updater::run(listener, "localhost", "1492");
		});
		while (gui.update()) {
			//std::this_thread::sleep_for(std::chrono::milliseconds(5));
		}
	} catch (std::exception &e) {
		std::cout << "Exception " << e.what() << std::endl;
	} catch (...) {
		std::cout << "Exception!" << std::endl;
	}
	return 0;
}
