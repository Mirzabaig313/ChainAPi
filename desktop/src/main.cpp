// Desktop entry point. Boots the QApplication and hands off to App.
#include "application/App.h"

#include <QtWidgets/QApplication>

int main(int argc, char** argv) {
    QApplication qt_app(argc, argv);
    qt_app.setApplicationName(QStringLiteral("ChainAPI"));
    qt_app.setApplicationVersion(QStringLiteral("0.1.0"));
    qt_app.setOrganizationName(QStringLiteral("ChainAPI"));

    chainapi::desktop::App app;
    app.show();
    return QApplication::exec();
}
