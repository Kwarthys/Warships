using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class ShipAnimator : MonoBehaviour
{
    [SerializeField]
    private Vector2 amps = Vector3.one;
    [SerializeField]
    private Vector2 speeds = Vector3.one;

    [SerializeField]
    private float upSpeed = 1;
    [SerializeField]
    private float upAmp = 1;

    private float dtCounter = 0f;

    public bool animate = false;

    private void Start()
    {
        dtCounter += Random.value;
    }


    void Update()
    {
        if (!animate) return;

        dtCounter += Time.deltaTime;

        Vector3 pos = transform.position;
        pos.y = Mathf.Sin(upSpeed * dtCounter) * upAmp;
        transform.position = pos;

        float x = Mathf.Sin(speeds.x * dtCounter) * amps.x;
        float z = Mathf.Sin(speeds.y * dtCounter) * amps.y;

        transform.localRotation = Quaternion.Euler(new Vector3(x, transform.localEulerAngles.y, z));
    }
}
