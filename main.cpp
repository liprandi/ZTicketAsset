
#include <QTranslator>
#include <QLocale>
#include <QApplication>
#include <QSettings>
#include <QFile>
#include <QFontDatabase>

#include "mainwindow.h"
#include "zdatabase.h"

#include <optional>

std::optional<std::string> getArgValue(int argc, char* argv[], const std::string& key) {
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];

        // Handle "--key=value"
        if (arg.rfind(key + "=", 0) == 0) {
            return arg.substr(key.length() + 1);
        }

        // Handle "--key value"
        if (arg == key && i + 1 < argc) {
            return argv[i + 1];
        }
    }
    return std::nullopt;
}

bool hasArg(int argc, char* argv[], const std::string& key) {
    return std::any_of(argv + 1, argv + argc, [&key](const char* arg) {
        return key == arg;
    });
}

int main(int argc, char *argv[]) {

    QLocale::setDefault(QLocale(QLocale::English, QLocale::UnitedStates));

    QApplication a(argc, argv);

    QSettings settings;

    QTranslator translator;
    if (translator.load(":/translations/app_it.qm")) {
        a.installTranslator(&translator);
    }

    auto local = hasArg(argc, argv, "--local") || hasArg(argc, argv, "-l");
    auto php  = getArgValue(argc, argv, "--php");

    if(local)
        ZDatabase::setPhpAddress(settings.value("php_address", "http://10.40.20.223/ticket.php").toString());
    else {
        if(!php) {
            ZDatabase::setPhpAddress(settings.value("php_address", "https://www.liprandi.com/ticket/liprandi_ticket.php").toString());
        }
        else {
            ZDatabase::setPhpAddress(php->c_str());
        }
    }

    // Load from resource (recommended)
    int fontId = QFontDatabase::addApplicationFont(":/fonts/NotoColorEmoji");
    QString fontFamily("Emoji");

    if (fontId == -1) {
        qWarning() << "Failed to load font!";
    } else {
        fontFamily = QFontDatabase::applicationFontFamilies(fontId).at(0);
    }

    QFile file(":/styles/dark");
    if(file.open(QFile::ReadOnly | QFile::Text)) {
        QString buff = QTextStream(&file).readAll();
        buff.replace("Emoji", fontFamily, Qt::CaseInsensitive);
        qApp->setStyleSheet(buff);
    }

    MainWindow w;
    w.show();

    return a.exec();
}
