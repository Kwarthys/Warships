using System.Collections;
using System.Collections.Generic;
using UnityEngine;
using UnityEngine.UI;
using UnityEngine.Events;
using UnityEngine.EventSystems;
using UnityEngine.Serialization;
using System;

public class CustomToggleButtonManager : Selectable, IPointerClickHandler, ISubmitHandler
{
    [SerializeField]
    private Image buttonImage;
    [SerializeField]
    private Sprite spriteON;
    [SerializeField]
    private Sprite spriteOFF;
    [SerializeField]
    private bool isOn = false;

    /*** copy pasted from Unity Toggles and TmproButtons ***/
    [Serializable]
    /// <summary>
    /// Function definition for a button click event.
    /// </summary>
    public class ButtonClickedEvent : UnityEvent<bool> { }

    // Event delegates triggered on click.
    [FormerlySerializedAs("onClick")]
    [SerializeField]
    private ButtonClickedEvent m_OnClick = new ButtonClickedEvent();
    
    public virtual void OnPointerClick(PointerEventData eventData)
    {
        if (eventData.button != PointerEventData.InputButton.Left)
            return;

        toggle();
    }

    public virtual void OnSubmit(BaseEventData eventData)
    {
        toggle();
    }
    /*******************************************************/

    protected override void Start()
    {
        updateSprites();
    }

    private void toggle()
    {
        isOn = !isOn;
        m_OnClick.Invoke(isOn);
        updateSprites();

    }

    private void updateSprites()
    {
        if(isOn)
        {
            buttonImage.sprite = spriteON;
        }
        else
        {
            buttonImage.sprite = spriteOFF;
        }
    }
}
