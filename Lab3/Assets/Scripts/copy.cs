using UnityEngine;

public class Bullet22 : MonoBehaviour
{
    public ScoreManager scoreManager; // Reference to your ScoreManager


    void OnCollisionEnter(Collision collision)
    {
        // Check if the collision is with an enemy
        if (collision.gameObject.CompareTag("Enemy"))
        {
            // Handle the collision with the enemy
            HandleEnemyCollision(collision.gameObject);

            // Destroy the bullet
            Destroy(gameObject);
        }
    }

    void HandleEnemyCollision(GameObject enemy)
    {

        // Destroy the enemy
        Destroy(enemy);

        // Increment the score
        if (scoreManager != null)
        {
            scoreManager.IncrementScore();
        }

    }
}
