#include "pch.h"
#include "processhelper.h"

ProcessHelper::ProcessHelper(QObject* parent) : QObject(parent) {
    _handle = new QProcess();
}

ProcessHelper::ProcessHelper(const QStringList& clArgs, const QString& procExe) : QObject(nullptr), _clArgs(clArgs), _procExe(procExe) {
    _handle = new QProcess(this);
    connect(_handle, &QProcess::finished, this, &ProcessHelper::onProcessFinished);
    _handle->setCreateProcessArgumentsModifier([](QProcess::CreateProcessArguments *args) {
        args->flags |= CREATE_NO_WINDOW;
    });
}

void ProcessHelper::setClArgs(const QStringList& args) {
    _clArgs = args;
}

void ProcessHelper::run() const {
    _handle->setWorkingDirectory(QCoreApplication::applicationDirPath());
    _handle->setArguments(_clArgs);
    _handle->start(_procExe);
}

void ProcessHelper::terminate() const {
    _handle->terminate();
}

QString ProcessHelper::getProcStdOut() const {
    return _handle->readAllStandardOutput();
}

void ProcessHelper::onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) {
    auto data = _handle->readAll();
    emit processFinished(exitCode, QString(data));
}