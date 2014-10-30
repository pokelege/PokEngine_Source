/****************************************************************************
** Meta object code from reading C++ file 'ConverterWindow.h'
**
** Created: Mon Oct 13 22:24:52 2014
**      by: The Qt Meta Object Compiler version 63 (Qt 4.8.0)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ConverterWindow.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ConverterWindow.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.0. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ConverterWindow[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
      10,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      17,   16,   16,   16, 0x08,
      26,   16,   16,   16, 0x08,
      44,   16,   16,   16, 0x08,
      71,   65,   16,   16, 0x08,
      96,   16,   16,   16, 0x08,
     112,   16,   16,   16, 0x08,
     131,   16,   16,   16, 0x08,
     150,   16,   16,   16, 0x08,
     170,   16,   16,   16, 0x08,
     188,   16,   16,   16, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ConverterWindow[] = {
    "ConverterWindow\0\0browse()\0startConverting()\0"
    "colorTextureButton()\0index\0"
    "currentIndexChanged(int)\0addFrameRange()\0"
    "deleteFrameRange()\0changeParamsNext()\0"
    "changeParamsStart()\0changeParamsEnd()\0"
    "save()\0"
};

void ConverterWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        ConverterWindow *_t = static_cast<ConverterWindow *>(_o);
        switch (_id) {
        case 0: _t->browse(); break;
        case 1: _t->startConverting(); break;
        case 2: _t->colorTextureButton(); break;
        case 3: _t->currentIndexChanged((*reinterpret_cast< int(*)>(_a[1]))); break;
        case 4: _t->addFrameRange(); break;
        case 5: _t->deleteFrameRange(); break;
        case 6: _t->changeParamsNext(); break;
        case 7: _t->changeParamsStart(); break;
        case 8: _t->changeParamsEnd(); break;
        case 9: _t->save(); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData ConverterWindow::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject ConverterWindow::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ConverterWindow,
      qt_meta_data_ConverterWindow, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ConverterWindow::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ConverterWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ConverterWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ConverterWindow))
        return static_cast<void*>(const_cast< ConverterWindow*>(this));
    return QWidget::qt_metacast(_clname);
}

int ConverterWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 10)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 10;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
