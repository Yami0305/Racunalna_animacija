using UnityEngine;

public class Bullet : MonoBehaviour
{
    public ScoreManager scoreManager;

    void OnCollisionEnter(Collision collision)
    {
        if (collision.gameObject.CompareTag("Enemy"))
        {
            HandleEnemyCollision(collision.gameObject);
        }
    }

    void HandleEnemyCollision(GameObject enemy)
    {
        // Assuming the sphere is the first child of the balloon
        Transform sphereTransform = enemy.transform.GetChild(0);

        // Access the Material component of the sphere
        Renderer sphereRenderer = sphereTransform.GetComponent<Renderer>();
        if (sphereRenderer != null)
        {
            Material sphereMaterial = sphereRenderer.material;

            // Access the layer of the bullet
            int bulletLayer = gameObject.layer;

            // Access the layer of the sphere (assuming it's set to the color layer)
            int sphereLayer = sphereTransform.gameObject.layer;

            // Check if the layers match
            if (bulletLayer == sphereLayer)
            {
                Destroy(enemy.transform.parent.gameObject);
                // Add other logic as needed for matching layers.
                if (scoreManager != null)
                {
                    scoreManager.IncrementScore();
                }
            }
            else
            {
                UnityEngine.Debug.Log("Bullet layer does not match sphere layer");
            }
        }
        else
        {
            UnityEngine.Debug.LogError("Sphere object is missing Renderer component");
        }
    }
}
