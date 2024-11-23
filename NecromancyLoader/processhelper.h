#pragma once

class ProcessHelper final : public QObject {
    Q_OBJECT

public:
    ProcessHelper(QObject *parent = nullptr);
    ProcessHelper(const QStringList& clArgs, const QString& procExe);

    void setClArgs(const QStringList& args);

    void run() const;
    void terminate() const;

    QString getProcStdOut() const;

signals:
    void processFinished(int exitCode, const QString& output);

private slots:
    void onProcessFinished(int exitCode, QProcess::ExitStatus exitStatus);

private:
    QStringList _clArgs;
    QString _procExe;
    QProcess* _handle;
};
