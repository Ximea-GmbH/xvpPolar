#ifndef POLARPLUGIN_H
#define POLARPLUGIN_H

#include <Plugin.h>
#include <QObject>

class CxPolarPlugin: public QObject, public CxPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "com.ximea.CxPolarPlugin")
    Q_INTERFACES(CxPlugin)

public:
    virtual bool init();
    virtual QString copyrightNotice() const;
    virtual QString name() const;
    virtual QString author() const;
    virtual QString description() const;
    virtual QString version() const;
    void version(int &major, int &minor, int &build) const;
};

#endif // POLARPLUGIN_H
