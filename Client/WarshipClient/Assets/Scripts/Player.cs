using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Player : MonoBehaviour
{
    public GridManager gridManager {get; private set;}
    public TargetingManager targetingManager { get; private set; }

    public string playerName { get; private set; } = "Player";

    public int playerID { get; private set; }

    private bool initialized = false;

    private bool isLocal = false;

    public void initPlayer(int playerID, Vector3 gridCenter, bool isLocal)
    {
        GameObject gridHolder = new GameObject("GridHolderP" + playerID);
        gridHolder.transform.position = gridCenter;
        gridHolder.transform.SetParent(transform);
        gridManager = gridHolder.AddComponent<GridManager>();
        gridManager.initialize();
        targetingManager = gameObject.AddComponent<TargetingManager>();
        targetingManager.isLocal = isLocal;

        this.playerID = playerID;
        initialized = true;
    }

    public void setPlayerName(string name)
    {
        playerName = name;
    }
}
