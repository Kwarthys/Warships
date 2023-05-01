using System.Collections;
using System.Collections.Generic;
using UnityEngine;

[CreateAssetMenu(fileName ="Ship", menuName = "ScriptableObjects/Ship", order = 1)]
public class ShipScriptableObject : ScriptableObject
{
    public string shipName;

    public Ship.ShipType type;
    public GameObject shipPrefab;
}
