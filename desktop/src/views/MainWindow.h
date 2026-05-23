#pragma once

#include <QtWidgets/QMainWindow>

namespace chainapi::desktop {

/// Shell window. Phase 2 fills in the explorer / request editor / response
/// viewer / timeline layout per PRD §9.1.
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget* parent = nullptr);
    ~MainWindow() override;
};

}  // namespace chainapi::desktop
