using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using TMPro;
using System.Text;

public class DebugTextManager : MonoBehaviour
{
    public static DebugTextManager instance;
    private void Awake()
    {
        instance = this;
    }

    private TextMeshProUGUI textMesh;
    private StringBuilder builder;

    private List<string> messages = new List<string>();

    private int maxMessagesNumber = 5;

    private void Start()
    {
        builder = new StringBuilder();
        textMesh = GetComponent<TextMeshProUGUI>();
    }

    private void updateTextMesh()
    {
        builder.Clear();
        for (int i = 0; i < messages.Count; ++i)
        {
            if(i!=0)builder.Append("\n");
            builder.Append(messages[i]);
        }
        textMesh.text = builder.ToString();
    }

    public void sendTextToDebug(string text)
    {
        trimList(maxMessagesNumber - 1);
        messages.Add(text);
        updateTextMesh();
    }

    /// <summary>
    /// Trims the internal list to the desired length
    /// </summary>
    /// <returns>Number of items removed</returns>
    private int trimList(int cap)
    {
        int removed = 0;
        while (messages.Count > cap)
        {
            messages.RemoveAt(0);
            removed++;
        }

        return removed;
    }


}
