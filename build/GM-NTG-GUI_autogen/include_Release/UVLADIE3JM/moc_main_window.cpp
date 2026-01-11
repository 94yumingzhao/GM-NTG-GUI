/****************************************************************************
** Meta object code from reading C++ file 'main_window.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../src/main_window.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'main_window.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 69
#error "This file was generated using the moc from 6.10.1. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

#ifndef Q_CONSTINIT
#define Q_CONSTINIT
#endif

QT_WARNING_PUSH
QT_WARNING_DISABLE_DEPRECATED
QT_WARNING_DISABLE_GCC("-Wuseless-cast")
namespace {
struct qt_meta_tag_ZN10MainWindowE_t {};
} // unnamed namespace

template <> constexpr inline auto MainWindow::qt_create_metaobjectdata<qt_meta_tag_ZN10MainWindowE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "MainWindow",
        "OnBrowseFile",
        "",
        "OnLoadResultFile",
        "OnStartSolver",
        "OnCancelSolver",
        "OnDataLoaded",
        "plants",
        "items",
        "families",
        "periods",
        "OnGrandCoalitionComputed",
        "cost",
        "runtime",
        "OnIterationStarted",
        "iteration",
        "OnMPSolved",
        "QList<double>",
        "pi",
        "v",
        "OnSPResult",
        "coalition",
        "violation",
        "OnConverged",
        "epsilon",
        "OnCutAdded",
        "OnSolverFinished",
        "success",
        "message",
        "final_v",
        "iterations",
        "OnLogMessage",
        "OnExportLog",
        "OnTabChanged",
        "index"
    };

    QtMocHelpers::UintData qt_methods {
        // Slot 'OnBrowseFile'
        QtMocHelpers::SlotData<void()>(1, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnLoadResultFile'
        QtMocHelpers::SlotData<void()>(3, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnStartSolver'
        QtMocHelpers::SlotData<void()>(4, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnCancelSolver'
        QtMocHelpers::SlotData<void()>(5, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnDataLoaded'
        QtMocHelpers::SlotData<void(int, int, int, int)>(6, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 7 }, { QMetaType::Int, 8 }, { QMetaType::Int, 9 }, { QMetaType::Int, 10 },
        }}),
        // Slot 'OnGrandCoalitionComputed'
        QtMocHelpers::SlotData<void(double, double)>(11, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 12 }, { QMetaType::Double, 13 },
        }}),
        // Slot 'OnIterationStarted'
        QtMocHelpers::SlotData<void(int)>(14, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 15 },
        }}),
        // Slot 'OnMPSolved'
        QtMocHelpers::SlotData<void(const QVector<double> &, double, double)>(16, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { 0x80000000 | 17, 18 }, { QMetaType::Double, 19 }, { QMetaType::Double, 13 },
        }}),
        // Slot 'OnSPResult'
        QtMocHelpers::SlotData<void(const QString &, double, double)>(20, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 21 }, { QMetaType::Double, 12 }, { QMetaType::Double, 22 },
        }}),
        // Slot 'OnConverged'
        QtMocHelpers::SlotData<void(double)>(23, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Double, 24 },
        }}),
        // Slot 'OnCutAdded'
        QtMocHelpers::SlotData<void(const QString &, double)>(25, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 21 }, { QMetaType::Double, 12 },
        }}),
        // Slot 'OnSolverFinished'
        QtMocHelpers::SlotData<void(bool, const QString &, double, int, double)>(26, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Bool, 27 }, { QMetaType::QString, 28 }, { QMetaType::Double, 29 }, { QMetaType::Int, 30 },
            { QMetaType::Double, 13 },
        }}),
        // Slot 'OnLogMessage'
        QtMocHelpers::SlotData<void(const QString &)>(31, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::QString, 28 },
        }}),
        // Slot 'OnExportLog'
        QtMocHelpers::SlotData<void()>(32, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnTabChanged'
        QtMocHelpers::SlotData<void(int)>(33, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 34 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<MainWindow, qt_meta_tag_ZN10MainWindowE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject MainWindow::staticMetaObject = { {
    QMetaObject::SuperData::link<QMainWindow::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN10MainWindowE_t>.metaTypes,
    nullptr
} };

void MainWindow::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<MainWindow *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->OnBrowseFile(); break;
        case 1: _t->OnLoadResultFile(); break;
        case 2: _t->OnStartSolver(); break;
        case 3: _t->OnCancelSolver(); break;
        case 4: _t->OnDataLoaded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 5: _t->OnGrandCoalitionComputed((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 6: _t->OnIterationStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 7: _t->OnMPSolved((*reinterpret_cast<std::add_pointer_t<QList<double>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 8: _t->OnSPResult((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 9: _t->OnConverged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 10: _t->OnCutAdded((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 11: _t->OnSolverFinished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5]))); break;
        case 12: _t->OnLogMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 13: _t->OnExportLog(); break;
        case 14: _t->OnTabChanged((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 7:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        }
    }
}

const QMetaObject *MainWindow::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *MainWindow::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN10MainWindowE_t>.strings))
        return static_cast<void*>(this);
    return QMainWindow::qt_metacast(_clname);
}

int MainWindow::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QMainWindow::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 15)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 15;
    }
    return _id;
}
QT_WARNING_POP
