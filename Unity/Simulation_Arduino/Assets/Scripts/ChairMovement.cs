using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ChairMovement : MonoBehaviour
{
    public float forwSpeed;
    //public float backSpeed;
    
    public float lerpTime = .25f;
    public Vector3 eulerScale = new Vector3(0, -1, 0);
    private Vector3 _rotOffset;

    private Quaternion _rot;
    public float _tiltThreshold = 20f;

    private int _gestureId;
    public int gestureId
    {
        set
        {
            this._gestureId = value;
            processGesture(_gestureId);
        }
    }
    
    private float _moveState;
    /*
    public delegate void GestureHandler(int gestureId);

    public GestureHandler gestureStarted;
    public GestureHandler gestureFinished;
    */
    private void Start()
    {
        _rotOffset = transform.rotation.eulerAngles;
        Connector.instance.DataReceived += DataReceived2;
    }

    private void Update()
    {
        _rot = Quaternion.Lerp(transform.rotation, _rot, lerpTime);
        transform.Translate(transform.right * (_moveState * forwSpeed * Time.deltaTime));
        transform.rotation = _rot;
    }

    public void DataReceived2(Quaternion q)
    {
        Vector3 euler = q.eulerAngles;
        checkForGesture(euler);
        euler = Vector3.Scale(euler, eulerScale) + _rotOffset;
        _rot = Quaternion.Euler(euler);
    }
    private void checkForGesture(Vector3 r) 
    {
        //Debug.Log(r);
        if (180 > r.x && r.x > _tiltThreshold) {
            gestureId = 1;
        }else if (180 < r.x && r.x < 360-_tiltThreshold) {
            gestureId = 2;
        }else if (180 > r.z && r.z > _tiltThreshold) {
            gestureId = 3;
        }else if (180 < r.z && r.z < 360-_tiltThreshold) {
            gestureId = 4;
        }else {
            gestureId = 0;
        }
    }
    
    public void processGesture(int gestureId) {
        _moveState = 0;
        switch (gestureId)
        {
            case 0:
                 break;
            case 1: break;
            case 2: break;
            case 3:
                _moveState = -1f; break;
            case 4:
                _moveState = 1f; break;
        }
    }
}