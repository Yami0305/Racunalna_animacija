using UnityEngine;
using Valve.VR;

public class VRShooting : MonoBehaviour
{
    public GameObject[] bulletPrefabs; // Array of different bullet models
    public Transform rightBarrelTransform;
    public Transform leftBarrelTransform;
    public float bulletSpeed = 20f;
    public float bulletLifetime = 3f; // Adjust the lifetime as needed
    public BulletColorDisplay colorDisplay;

    private int currentBulletIndex = 0; // Index of the current bullet model

    private void Update()
    {
        // Check for shooting input on the right hand
        if (SteamVR_Input.GetStateDown("Shoot", SteamVR_Input_Sources.RightHand))
        {
            Shoot(rightBarrelTransform);
        }

        // Check for shooting input on the left hand
        if (SteamVR_Input.GetStateDown("Shoot", SteamVR_Input_Sources.LeftHand))
        {
            Shoot(leftBarrelTransform);
        }

        // Check for input to switch bullet models
        if (SteamVR_Input.GetStateDown("SwitchColor", SteamVR_Input_Sources.Any))
        {
            SwitchBulletModel();
        }
    }

    private void Shoot(Transform barrelTransform)
    {
        // Instantiate a bullet at the barrel position and rotation
        GameObject bullet = Instantiate(bulletPrefabs[currentBulletIndex], barrelTransform.position, barrelTransform.rotation);

        // Get the bullet's rigidbody
        Rigidbody bulletRb = bullet.GetComponent<Rigidbody>();

        // Apply force to the bullet in the forward direction
        bulletRb.velocity = barrelTransform.forward * bulletSpeed;

        // Destroy the bullet after a specified lifetime
        Destroy(bullet, bulletLifetime);
    }

    private void SwitchBulletModel()
    {
        // Increment the bullet model index (cycle back to 0 if reaching the end)
        currentBulletIndex = (currentBulletIndex + 1) % bulletPrefabs.Length;

        Color newColor = bulletPrefabs[currentBulletIndex].GetComponent<Renderer>().material.color;
        colorDisplay.SetColorDisplay(newColor);
    }
}
