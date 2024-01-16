using UnityEngine;

public class CircularSpawnArea : MonoBehaviour
{
    public GameObject[] enemyPrefabs; // Array of enemy prefabs
    public float spawnRadius = 10f;

    public float minSpawnInterval = 1f;
    public float maxSpawnInterval = 5f;

    private float timeSinceLastSpawn;

    void Start()
    {
        timeSinceLastSpawn = 0f;
        InvokeRepeating("SpawnEnemy", 0f, UnityEngine.Random.Range(minSpawnInterval, maxSpawnInterval));
    }

    void SpawnEnemy()
    {
        float spawnAngle = UnityEngine.Random.Range(0f, 360f);
        float angle = Mathf.Deg2Rad * spawnAngle;

        Vector3 spawnPosition = new Vector3(
            transform.position.x + Mathf.Cos(angle) * spawnRadius,
            transform.position.y,
            transform.position.z + Mathf.Sin(angle) * spawnRadius
        );

        // Choose a random enemy prefab
        GameObject selectedEnemyPrefab = ChooseRandomEnemyPrefab();

        // Print the name of the selected enemy prefab
        UnityEngine.Debug.Log("Selected Enemy: " + selectedEnemyPrefab.name);

        // Spawn the selected enemy
        GameObject enemy = Instantiate(selectedEnemyPrefab, spawnPosition, Quaternion.identity);

        // Assign a speed based on the name of the selected enemy prefab
        float speed = GetSpeedFromPrefabName(selectedEnemyPrefab.name);

        // Check if the EnemyMovement component already exists
        EnemyMovement enemyMovement = enemy.GetComponent<EnemyMovement>();
        if (enemyMovement == null)
        {
            // If not, add a new EnemyMovement component
            enemyMovement = enemy.AddComponent<EnemyMovement>();
        }

        // Initialize the EnemyMovement component
        enemyMovement.Initialize(transform.position, speed);
    }

    float GetSpeedFromPrefabName(string prefabName)
    {
        if (prefabName == "Baloon Red")
        {
            return 1.0f;
        }
        else if (prefabName == "Baloon Blue")
        {
            return 1.5f;
        }
        else if (prefabName == "Baloon Yellow")
        {
            return 2.5f;
        }
        else if (prefabName == "Baloon Black")
        {
            return 3.5f;
        }
        return 1.5f;
    }

    GameObject ChooseRandomEnemyPrefab()
    {
        float randomValue = UnityEngine.Random.value; // Random value between 0 and 1

        // Adjust these probabilities based on your desired distribution
        float redProbability = 0.5f;
        float blueProbability = 0.3f;
        float yellowProbability = 0.15f;
        float blackProbability = 0.05f;

        if (randomValue < redProbability)
        {
            return enemyPrefabs[0]; // Index of red balloon
        }
        else if (randomValue < redProbability + blueProbability)
        {
            return enemyPrefabs[1]; // Index of blue balloon
        }
        else if (randomValue < redProbability + blueProbability + yellowProbability)
        {
            return enemyPrefabs[2]; // Index of yellow balloon
        }
        else
        {
            return enemyPrefabs[3]; // Index of black balloon
        }
    }

}
