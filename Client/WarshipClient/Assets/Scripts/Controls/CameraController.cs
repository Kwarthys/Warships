using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class CameraController : MonoBehaviour
{
    public float width = 10f;
    public float height = 10f;

    public Vector3 center = Vector3.zero;

    public float speed = 10f;

    public Camera theCamera;
    public Transform cameraTarget;

    private void Start()
    {
        theCamera.transform.LookAt(cameraTarget);

        transform.position = center;
    }

    private void Update()
    {
        float vertical = Input.GetAxisRaw("Vertical");
        float horizontal = Input.GetAxisRaw("Horizontal");

        if(vertical != 0f || horizontal != 0f)
        {
            vertical *= Time.deltaTime * speed;
            horizontal *= Time.deltaTime * speed;

            if(transform.position.x + horizontal < center.x - (width / 2f) || transform.position.x + horizontal > center.x + (width/ 2f))
            {
                horizontal = 0f;
            }
            if(transform.position.z + vertical < center.z - (height / 2f) || transform.position.z + vertical > center.z + (height / 2f))
            {
                vertical = 0f;
            }

            Vector3 move = new Vector3(horizontal, 0, vertical);
            transform.Translate(move);
        }
    }


    private void OnDrawGizmosSelected()
    {
        Vector3 halfHeight = Vector3.forward * height / 2;
        Vector3 halfWidth = Vector3.right * width / 2;

        //4 vertex rectangle, a b c and d
        //a ------ b
        //| center |
        //c ------ d
        Vector3 a = center - halfWidth + halfHeight;
        Vector3 b = center + halfWidth + halfHeight;
        Vector3 c = center - halfWidth - halfHeight;
        Vector3 d = center + halfWidth - halfHeight;

        Gizmos.DrawLine(a, b);
        Gizmos.DrawLine(b, d);
        Gizmos.DrawLine(a, c);
        Gizmos.DrawLine(c, d);
    }
}
