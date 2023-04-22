using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.VFX;

public class FXController : MonoBehaviour
{
    public VisualEffect fx { get; private set; }

    public void init()
    {
        fx = GetComponent<VisualEffect>();
        stop();
    }

    public virtual void play()
    {
        fx.Play();
    }

    public virtual void stop()
    {
        fx.Stop();
    }
}
