using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class HeadMovement : MonoBehaviour
{
    public float lerpTime = .25f;
    public Vector3 eulerScale = new Vector3(-1, -1, 1);
    private Vector3 rotOffset;

    void Start()
    {
        rotOffset = transform.rotation.eulerAngles;
        Connector.instance.DataReceived += DataReceived;
    }
    
    private void DataReceived(Quaternion q)
    {
        Vector3 euler = q.eulerAngles;
        euler = Vector3.Scale(euler, eulerScale) + rotOffset;
        q = Quaternion.Euler(euler);
        q = Quaternion.Lerp(transform.rotation, q, lerpTime);
        transform.rotation = q;
    }
}