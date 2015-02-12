//
//  AudioInjector.h
//  libraries/audio/src
//
//  Created by Stephen Birarda on 1/2/2014.
//  Copyright 2014 High Fidelity, Inc.
//
//  Distributed under the Apache License, Version 2.0.
//  See the accompanying file LICENSE or http://www.apache.org/licenses/LICENSE-2.0.html
//

#ifndef hifi_AudioInjector_h
#define hifi_AudioInjector_h

#include <QtCore/QObject>
#include <QtCore/QSharedPointer>
#include <QtCore/QThread>

#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

#include "AudioInjectorLocalBuffer.h"
#include "AudioInjectorOptions.h"
#include "Sound.h"

class AbstractAudioInterface;

class AudioInjector : public QObject {
    Q_OBJECT
public:
    AudioInjector(QObject* parent);
    AudioInjector(Sound* sound, const AudioInjectorOptions& injectorOptions);
    AudioInjector(const QByteArray& audioData, const AudioInjectorOptions& injectorOptions);
    
    bool isFinished() const { return _isFinished; }
    
    int getCurrentSendPosition() const { return _currentSendPosition; }
    
    AudioInjectorLocalBuffer* getLocalBuffer() const { return _localBuffer; }
    bool isLocalOnly() const { return _options.localOnly; }
    
    void setLocalAudioInterface(AbstractAudioInterface* localAudioInterface) { _localAudioInterface = localAudioInterface; }
public slots:
    void injectAudio();
    void stop();
    void setOptions(AudioInjectorOptions& options) { _options = options; }
    void setCurrentSendPosition(int currentSendPosition) { _currentSendPosition = currentSendPosition; }
    float getLoudness() const { return _loudness; }
    bool isPlaying() const { return !_isFinished; }
    
signals:
    void finished();

private slots:
    void stopAndDeleteLater();
private:
    void injectToMixer();
    void injectLocally();
    
    void setIsFinished(bool isFinished);
    
    QByteArray _audioData;
    AudioInjectorOptions _options;
    bool _shouldStop = false;
    float _loudness = 0.0f;
    bool _isStarted = false;
    bool _isFinished = false;
    int _currentSendPosition = 0;
    AbstractAudioInterface* _localAudioInterface = NULL;
    AudioInjectorLocalBuffer* _localBuffer = NULL;
    
    friend QScriptValue injectorToScriptValue(QScriptEngine* engine, AudioInjector* const& in);
};

Q_DECLARE_METATYPE(AudioInjector*)

QScriptValue injectorToScriptValue(QScriptEngine* engine, AudioInjector* const& in);
void injectorFromScriptValue(const QScriptValue& object, AudioInjector*& out);

#endif // hifi_AudioInjector_h
