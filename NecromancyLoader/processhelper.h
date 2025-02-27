#pragma once

class ProcessHelper final : public QObject
{
    Q_OBJECT

public:
    ProcessHelper(QObject* parent = nullptr);
    ProcessHelper(const QStringList& clArgs, const QString& procExe);

    void setClArgs(const QStringList& args);
    void setTargetProc(const QString& exe);

    void run() const;
    void terminate() const;

    QString getProcStdOut() const;

signals:
    void processFinished(int exitCode, const QString& output) const;

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus) const;

private:
    QStringList _clArgs;
    QString _procExe;
    QProcess* _handle;
};
