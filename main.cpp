#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QSettings>
#include <QQuickStyle>
#include <QQuickItem>
#include <QIcon>
#include <QVariant>
#include "zdatabase.h"
#include "zbackend.h"
#include "ztable.h"

QObject *qmlObj = 0l;

int main(int argc, char *argv[])
{
    QGuiApplication::setApplicationName("Ticket Asset");
    QGuiApplication::setOrganizationName("CamDoBrasil");

 //   qputenv("QT_IM_MODULE", QByteArray("qtvirtualkeyboard"));

    QGuiApplication app(argc, argv);

    QIcon::setThemeName("zticketasset");

    QSettings settings;
    if(qEnvironmentVariableIsEmpty("QT_QUICK_CONTROLS_STYLE"))
        QQuickStyle::setStyle(settings.value("style").toString());

    ZDatabase::setPhpAddress(settings.value("php_address", "https://www.liprandi.com/ticket/liprandi_ticket.php").toString());

    QQmlApplicationEngine engine;
    // Register your QAbstractTableModel subclass
    qmlRegisterType<ZTable>("ZTicketAsset", 1, 0, "ZTable");
    qmlRegisterType<ZBackEnd>("ZTicketAsset", 1, 0, "ZBackEnd");

    QStringList builtInStyles = { QLatin1String("Basic"), QLatin1String("Fusion"),
                                 QLatin1String("Imagine"), QLatin1String("Material"), QLatin1String("Universal"),
                                 QLatin1String("FluentWinUI3") };

    engine.setInitialProperties({{ "builtInStyles", builtInStyles }});
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);

    engine.load(QUrl("qrc:/zticketasset.qml"));

    if(engine.rootObjects().isEmpty())
        return -1;

    qmlObj = engine.rootObjects()[0];

    return app.exec();
}
