#include "App.h"

#include "Bootstrapper.h"
#include "../views/MainWindow.h"

namespace chainapi::desktop {

App::App()
    : bootstrapper_(std::make_unique<Bootstrapper>()),
      main_window_(std::make_unique<MainWindow>()) {}

App::~App() = default;

void App::show() {
    main_window_->show();
}

}  // namespace chainapi::desktop
