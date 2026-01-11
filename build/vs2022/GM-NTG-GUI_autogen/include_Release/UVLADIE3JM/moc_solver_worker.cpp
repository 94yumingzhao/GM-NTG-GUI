/****************************************************************************
** Meta object code from reading C++ file 'solver_worker.h'
**
** Created by: The Qt Meta Object Compiler version 69 (Qt 6.10.1)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../../../src/solver_worker.h"
#include <QtCore/qmetatype.h>
#include <QtCore/QList>

#include <QtCore/qtmochelpers.h>

#include <memory>


#include <QtCore/qxptype_traits.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'solver_worker.h' doesn't include <QObject>."
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
struct qt_meta_tag_ZN12SolverWorkerE_t {};
} // unnamed namespace

template <> constexpr inline auto SolverWorker::qt_create_metaobjectdata<qt_meta_tag_ZN12SolverWorkerE_t>()
{
    namespace QMC = QtMocConstants;
    QtMocHelpers::StringRefStorage qt_stringData {
        "SolverWorker",
        "DataLoaded",
        "",
        "plants",
        "items",
        "families",
        "periods",
        "GrandCoalitionComputed",
        "cost",
        "runtime",
        "IterationStarted",
        "iteration",
        "MPSolved",
        "QList<double>",
        "pi",
        "v",
        "SPResult",
        "coalition",
        "violation",
        "Converged",
        "epsilon",
        "CutAdded",
        "Finished",
        "success",
        "message",
        "final_v",
        "iterations",
        "LogMessage",
        "Run",
        "solver_path",
        "data_file",
        "QVariantMap",
        "params",
        "OnReadyReadStdout",
        "OnReadyReadStderr",
        "OnProcessFinished",
        "exit_code",
        "QProcess::ExitStatus",
        "status"
    };

    QtMocHelpers::UintData qt_methods {
        // Signal 'DataLoaded'
        QtMocHelpers::SignalData<void(int, int, int, int)>(1, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 3 }, { QMetaType::Int, 4 }, { QMetaType::Int, 5 }, { QMetaType::Int, 6 },
        }}),
        // Signal 'GrandCoalitionComputed'
        QtMocHelpers::SignalData<void(double, double)>(7, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 8 }, { QMetaType::Double, 9 },
        }}),
        // Signal 'IterationStarted'
        QtMocHelpers::SignalData<void(int)>(10, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Int, 11 },
        }}),
        // Signal 'MPSolved'
        QtMocHelpers::SignalData<void(QVector<double>, double, double)>(12, 2, QMC::AccessPublic, QMetaType::Void, {{
            { 0x80000000 | 13, 14 }, { QMetaType::Double, 15 }, { QMetaType::Double, 9 },
        }}),
        // Signal 'SPResult'
        QtMocHelpers::SignalData<void(QString, double, double)>(16, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 }, { QMetaType::Double, 8 }, { QMetaType::Double, 18 },
        }}),
        // Signal 'Converged'
        QtMocHelpers::SignalData<void(double)>(19, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Double, 20 },
        }}),
        // Signal 'CutAdded'
        QtMocHelpers::SignalData<void(QString, double)>(21, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 17 }, { QMetaType::Double, 8 },
        }}),
        // Signal 'Finished'
        QtMocHelpers::SignalData<void(bool, QString, double, int, double)>(22, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::Bool, 23 }, { QMetaType::QString, 24 }, { QMetaType::Double, 25 }, { QMetaType::Int, 26 },
            { QMetaType::Double, 9 },
        }}),
        // Signal 'LogMessage'
        QtMocHelpers::SignalData<void(QString)>(27, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 24 },
        }}),
        // Slot 'Run'
        QtMocHelpers::SlotData<void(const QString &, const QString &, const QVariantMap &)>(28, 2, QMC::AccessPublic, QMetaType::Void, {{
            { QMetaType::QString, 29 }, { QMetaType::QString, 30 }, { 0x80000000 | 31, 32 },
        }}),
        // Slot 'OnReadyReadStdout'
        QtMocHelpers::SlotData<void()>(33, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnReadyReadStderr'
        QtMocHelpers::SlotData<void()>(34, 2, QMC::AccessPrivate, QMetaType::Void),
        // Slot 'OnProcessFinished'
        QtMocHelpers::SlotData<void(int, QProcess::ExitStatus)>(35, 2, QMC::AccessPrivate, QMetaType::Void, {{
            { QMetaType::Int, 36 }, { 0x80000000 | 37, 38 },
        }}),
    };
    QtMocHelpers::UintData qt_properties {
    };
    QtMocHelpers::UintData qt_enums {
    };
    return QtMocHelpers::metaObjectData<SolverWorker, qt_meta_tag_ZN12SolverWorkerE_t>(QMC::MetaObjectFlag{}, qt_stringData,
            qt_methods, qt_properties, qt_enums);
}
Q_CONSTINIT const QMetaObject SolverWorker::staticMetaObject = { {
    QMetaObject::SuperData::link<QObject::staticMetaObject>(),
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SolverWorkerE_t>.stringdata,
    qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SolverWorkerE_t>.data,
    qt_static_metacall,
    nullptr,
    qt_staticMetaObjectRelocatingContent<qt_meta_tag_ZN12SolverWorkerE_t>.metaTypes,
    nullptr
} };

void SolverWorker::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    auto *_t = static_cast<SolverWorker *>(_o);
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: _t->DataLoaded((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4]))); break;
        case 1: _t->GrandCoalitionComputed((*reinterpret_cast<std::add_pointer_t<double>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 2: _t->IterationStarted((*reinterpret_cast<std::add_pointer_t<int>>(_a[1]))); break;
        case 3: _t->MPSolved((*reinterpret_cast<std::add_pointer_t<QList<double>>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 4: _t->SPResult((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3]))); break;
        case 5: _t->Converged((*reinterpret_cast<std::add_pointer_t<double>>(_a[1]))); break;
        case 6: _t->CutAdded((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[2]))); break;
        case 7: _t->Finished((*reinterpret_cast<std::add_pointer_t<bool>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[3])),(*reinterpret_cast<std::add_pointer_t<int>>(_a[4])),(*reinterpret_cast<std::add_pointer_t<double>>(_a[5]))); break;
        case 8: _t->LogMessage((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1]))); break;
        case 9: _t->Run((*reinterpret_cast<std::add_pointer_t<QString>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QString>>(_a[2])),(*reinterpret_cast<std::add_pointer_t<QVariantMap>>(_a[3]))); break;
        case 10: _t->OnReadyReadStdout(); break;
        case 11: _t->OnReadyReadStderr(); break;
        case 12: _t->OnProcessFinished((*reinterpret_cast<std::add_pointer_t<int>>(_a[1])),(*reinterpret_cast<std::add_pointer_t<QProcess::ExitStatus>>(_a[2]))); break;
        default: ;
        }
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType(); break;
            case 0:
                *reinterpret_cast<QMetaType *>(_a[0]) = QMetaType::fromType< QList<double> >(); break;
            }
            break;
        }
    }
    if (_c == QMetaObject::IndexOfMethod) {
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(int , int , int , int )>(_a, &SolverWorker::DataLoaded, 0))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(double , double )>(_a, &SolverWorker::GrandCoalitionComputed, 1))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(int )>(_a, &SolverWorker::IterationStarted, 2))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(QVector<double> , double , double )>(_a, &SolverWorker::MPSolved, 3))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(QString , double , double )>(_a, &SolverWorker::SPResult, 4))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(double )>(_a, &SolverWorker::Converged, 5))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(QString , double )>(_a, &SolverWorker::CutAdded, 6))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(bool , QString , double , int , double )>(_a, &SolverWorker::Finished, 7))
            return;
        if (QtMocHelpers::indexOfMethod<void (SolverWorker::*)(QString )>(_a, &SolverWorker::LogMessage, 8))
            return;
    }
}

const QMetaObject *SolverWorker::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *SolverWorker::qt_metacast(const char *_clname)
{
    if (!_clname) return nullptr;
    if (!strcmp(_clname, qt_staticMetaObjectStaticContent<qt_meta_tag_ZN12SolverWorkerE_t>.strings))
        return static_cast<void*>(this);
    return QObject::qt_metacast(_clname);
}

int SolverWorker::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QObject::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 13)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 13;
    }
    return _id;
}

// SIGNAL 0
void SolverWorker::DataLoaded(int _t1, int _t2, int _t3, int _t4)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 0, nullptr, _t1, _t2, _t3, _t4);
}

// SIGNAL 1
void SolverWorker::GrandCoalitionComputed(double _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 1, nullptr, _t1, _t2);
}

// SIGNAL 2
void SolverWorker::IterationStarted(int _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 2, nullptr, _t1);
}

// SIGNAL 3
void SolverWorker::MPSolved(QVector<double> _t1, double _t2, double _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 3, nullptr, _t1, _t2, _t3);
}

// SIGNAL 4
void SolverWorker::SPResult(QString _t1, double _t2, double _t3)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 4, nullptr, _t1, _t2, _t3);
}

// SIGNAL 5
void SolverWorker::Converged(double _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 5, nullptr, _t1);
}

// SIGNAL 6
void SolverWorker::CutAdded(QString _t1, double _t2)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 6, nullptr, _t1, _t2);
}

// SIGNAL 7
void SolverWorker::Finished(bool _t1, QString _t2, double _t3, int _t4, double _t5)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 7, nullptr, _t1, _t2, _t3, _t4, _t5);
}

// SIGNAL 8
void SolverWorker::LogMessage(QString _t1)
{
    QMetaObject::activate<void>(this, &staticMetaObject, 8, nullptr, _t1);
}
QT_WARNING_POP
