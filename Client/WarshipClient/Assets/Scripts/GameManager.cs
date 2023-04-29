using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class GameManager : MonoBehaviour
{
    public static GameManager instance;
    private void Awake()
    {
        instance = this;
    }

    private CommandSender commandSender = new CommandSender();


    private List<Player> players = new List<Player>();
    private Player localPlayer; //shortcut to frequently used localPlayer, also present in players
    private List<int> playerIndexToID = new List<int>();

    [SerializeField]
    private ShipScriptableObject[] shipsToPlace;
    [SerializeField]
    private Ship[] ships;

    private int placingIndex = -1;
    private GameObject placingGhost = null;
    private int ghostLength;
    private bool donePlacing = false; //set true to bypass placement (usefull for debug)
    private bool otherDonePlacing = false;
    private bool nameSelection = true;

    [SerializeField]
    private GameObject flarePrefab;
    [SerializeField]
    private GameObject buoyPrefab;
    [SerializeField]
    private GameObject missBuoyPrefab;
    [SerializeField]
    private GameObject hitFXPrefab;
    [SerializeField]
    private GameObject sunkFXPrefab;
    public GameObject getFlarePrefab() { return flarePrefab; }
    public GameObject getBuoyPrefab() { return buoyPrefab; }
    public GameObject getHitFXPrefab() { return hitFXPrefab; }
    public GameObject getMissBuoyPrefab() { return missBuoyPrefab; }
    public GameObject getSunkFXPrefab() { return sunkFXPrefab; }

    [SerializeField]
    private Transform[] playerPlacements;

    [SerializeField]
    private Camera theCamera;

    [SerializeField]
    private LayerMask seaLayer;

    public string localPlayerName = "Player";

    public Player registerPlayer(int index, int playerPlacement)
    {
        GameObject playerObject = new GameObject("Player" + index + "Object");
#if UNITY_EDITOR
        playerObject.transform.SetParent(transform);//keeping editor hierarchy clear, only in the editor
#endif
        Player player = playerObject.AddComponent<Player>();
        player.initPlayer(index, playerPlacements[playerPlacement].position);

        playerIndexToID.Add(index);
        players.Add(player);

        return player;
    }

    public void registerLocalPlayerID(int id, int playerPlacement)
    {
        localPlayer = registerPlayer(id, playerPlacement);
        DebugTextManager.instance.sendTextToDebug("LocalPlayerID: " + id);

        ScoreDisplayManager.instance.registerNewPlayer(id, localPlayerName, 5, true);
    }

    public void registerPlayerName(int playerID, string playerName)
    {
        if(playerID != localPlayer.playerID)
        {
            getPlayerByID(playerID).setPlayerName(playerName);
            ScoreDisplayManager.instance.registerNewPlayer(playerID, playerName, 5);
        }
    }

    void Start()
    {
        ships = new Ship[shipsToPlace.Length];

        commandSender.sendNameCommand(localPlayerName);
    }

    public void gameStarts()
    {
        otherDonePlacing = true;
    }

    // Update is called once per frame
    void Update()
    {
        if (localPlayer == null) return;
        if (nameSelection) return;

        if (!donePlacing)
        {
            manageShipPlacement();
        }
        else if(otherDonePlacing)
        {
            manageTargeting();
        }        
    }

    public bool tryGetMousePosOnBoard(out Vector3 point)
    {
        Ray ray = theCamera.ScreenPointToRay(Input.mousePosition);

        float maxDistance = 250f;
        if (Physics.Raycast(ray, out RaycastHit hit, maxDistance, seaLayer))
        {
            point = hit.point;
            return true;
        }
        else
        {
            point = Vector3.zero;
            return false;
        }
    }

    private void manageTargeting()
    {
        if (Input.GetMouseButtonDown(0))
        {
            if(tryGetMousePosOnBoard(out Vector3 point))
            {
                for (int i = 0; i < players.Count; i++)
                {
                    if (players[i].gridManager.trySnapWorldToGrid(point, out Vector3 gridPos))
                    {
                        //This player is targeted by Local player
                        localPlayer.targetingManager.target(gridPos, players[i].playerID, players[i].gridManager.fromWorldToNode(gridPos));
                        //send target to network
                    }
                }
            }
        }
    }

    public void registerOpponentTargeting(int attackingPlayerId, int targetGridId, int[] targetNodeIds)
    {
        Player attacker = getPlayerByID(attackingPlayerId);
        Player attacked = getPlayerByID(targetGridId);

        for (int i = 0; i < targetNodeIds.Length; i++)
        {
            attacker.targetingManager.target(attacked.gridManager.fromIndexToWorld(targetNodeIds[i]), targetGridId, targetNodeIds[i]);
        }
    }

    private void manageShipPlacement()
    {
        if (placingGhost == null)
        {
            //starting or placing a new one
            ++placingIndex;
            if (placingIndex < shipsToPlace.Length)
            {
                placingGhost = Instantiate(shipsToPlace[placingIndex].shipPrefab);
                ghostLength = shipsToPlace[placingIndex].length;

                ships[placingIndex] = new Ship(shipsToPlace[placingIndex], placingIndex);
            }
            else
            {
                donePlacing = true;
                return;
            }

        }

        if (Input.GetKeyDown(KeyCode.R))
        {
            ships[placingIndex].rotate();
            placingGhost.transform.Rotate(new Vector3(0, 90, 0));
        }

        Vector3 point;
        if (tryGetMousePosOnBoard(out point))
        {
            if (localPlayer.gridManager.snapShipToGrid(point, out Vector3 pos, ships[placingIndex].length, ships[placingIndex].orientation))
            {
                placingGhost.transform.position = pos;

                if (Input.GetMouseButtonDown(0))//left clic
                {
                    if (localPlayer.gridManager.placeShipAt(point, ships[placingIndex]))
                    {
                        placingGhost.GetComponent<ShipAnimator>().animate = true;
                        placingGhost = null;

                        commandSender.sendPlaceShip((int)ships[placingIndex].orientation, localPlayer.gridManager.getRootNodeOfShip(point, ships[placingIndex]));
                    }
                }
            }            
        }
    }
    private Player getPlayerByID(int playerID)
    {
        for (int i = 0; i < playerIndexToID.Count; i++)
        {
            if(playerIndexToID[i] == playerID)
            {
                return players[i];
            }
        }

        Debug.LogError("Could not retreive Player by ID" + playerID);
        return null;
    }

    private bool tryGetPlayerByID(int playerID, out Player player)
    {
        for (int i = 0; i < playerIndexToID.Count; i++)
        {
            if (playerIndexToID[i] == playerID)
            {
                player = players[i];
                return true;
            }
        }

        Debug.LogError("Could not retreive Player by ID" + playerID);
        player = null;
        return false;
    }
}
