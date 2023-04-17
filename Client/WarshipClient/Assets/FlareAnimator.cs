using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.VFX;

public class FlareAnimator : MonoBehaviour
{
    public VisualEffect fx { get; private set; }

    [SerializeField]
    private float startHeight = 50f;
    [SerializeField]
    private float fallSpeed = 1f;

    private float fallT = 0f;

    public bool needsUpdate { get; private set; } = false;

    public void init()
    {
        fx = GetComponent<VisualEffect>();
        fx.Stop();
    }

    public void updateAnimation()
    {
        if (!needsUpdate)
        {
            Debug.LogWarning("Update should not be called when needs update is false");
            return;
        }

        Vector3 pos = transform.position;
        fallT += Time.deltaTime * fallSpeed;
        if(fallT >= 1)
        {
            pos.y = 0;
            needsUpdate = false;
        }
        else
        {
            pos.y = Mathf.Lerp(startHeight, 0, fallT * fallT);
        }

        transform.position = pos;

    }

    public void play()
    {
        needsUpdate = true;
        fallT = 0f;
        fx.Play();


        Vector3 pos = transform.position;
        pos.y = startHeight;
        transform.position = pos;
    }

    public void stop()
    {
        needsUpdate = false;
        fx.Stop();
    }
}
