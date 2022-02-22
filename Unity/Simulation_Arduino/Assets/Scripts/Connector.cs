using System;
using UnityEngine;
using System.IO.Ports;

public class Connector : MonoBehaviour
{
    private static string portName = "COM3";
    private SerialPort sp;

    private float timer;
    private bool calibrated = false;
    public delegate void ReceiveData(Quaternion quat);
    public ReceiveData DataReceived;
    public ReceiveData ConnectionLoss;

    public Transform chair;
    public static Connector instance;
    
    //
    public AudioSource _audioSource;

    public GameObject redFb;
    //

    private void Awake()
    {
        if(instance != null) Destroy(gameObject);
        instance = this;
    }

    private void Start()
    {
        if (SerialPort.GetPortNames().Length > 0)
        {
            portName = SerialPort.GetPortNames()[0];
        }
        else
        {
            Debug.Log("Port yok.");
            enabled = false;
        }
        sp = new SerialPort(portName, 115200);
        sp.Open();
        sp.ReadTimeout = 1; //http://answers.unity.com/answers/1140124/view.html
        sp.Write("r");
    }

    private void Update()
    {
        timer += Time.deltaTime;
        if (timer >= 1)
        {
            sp.Write("r");
            calibrated = false;
            timer = 0;
        }

        try
        {
            serialEvent();
        }
        catch (Exception e)
        {
        }
    }

    public void serialEvent()
    {
        string data = sp.ReadLine(); //the ascii value of the "|" character
        if (data != null)
        {
            timer = 0;
            
            data = data.Trim(); //remove whitespace around our values
            string[] inputs = data.Split('/');
            if (!inputs[0].Equals("quat")) { print($"{portName}: {data}");return;}

            //now assign your values in processing
            if (inputs.Length == 5)
            {
                if (!calibrated) { Debug.Log("CALIBRATED!!!"); calibrated = true;
                    CalibrateFeedback();
                }
                Quaternion q = new Quaternion();
                q.w = float.Parse(inputs[1]); 
                q.z = float.Parse(inputs[2]);
                q.x = float.Parse(inputs[3]);
                q.y = float.Parse(inputs[4]);
                DataReceived?.Invoke(q);
            }
        }

    }
    private void CalibrateFeedback()
    {
        _audioSource.Play();
        redFb.SetActive(false);
    }
}
