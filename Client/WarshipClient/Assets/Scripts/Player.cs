using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public GridManager gridManager {get; private set;}
    public TargetingManager targetingManager { get; private set; }

    public string playerName { get; private set; }

    public int playerID { get; private set; }

    private bool initialized = false;

    public void initPlayer(int playerID, string playerName, Vector3 gridCenter)
    {
        GameObject gridHolder = new GameObject("GridHolderP" + playerID);
        gridHolder.transform.SetParent(transform);
        gridManager = gridHolder.AddComponent<GridManager>();
        targetingManager = gameObject.AddComponent<TargetingManager>();

        this.playerID = playerID;
        this.playerName = playerName;
        initialized = true;
    }
}
